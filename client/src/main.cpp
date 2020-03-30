#include <iomanip>
#include <iostream>
#include <sstream>

#include <condition_variable>
#include <thread>

#include <vsomeip/vsomeip.hpp>

#define SAMPLE_SERVICE_ID 0x1234
#define SAMPLE_INSTANCE_ID 0x5678
#define SAMPLE_METHOD_ID 0x0421

std::shared_ptr< vsomeip::application > app;
std::mutex mutex;
std::condition_variable condition;

void run()
{
    std::unique_lock<std::mutex> its_lock(mutex);
    condition.wait(its_lock);

    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_service(SAMPLE_SERVICE_ID);
    request->set_instance(SAMPLE_INSTANCE_ID);
    request->set_method(SAMPLE_METHOD_ID);

    const int loop = 5;
    for (int i = 0; i < loop; i++)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get()->create_payload();
        std::string mydata("Hello server!");
        std::vector<vsomeip::byte_t> data(mydata.begin(), mydata.end());

        payload->set_data(data);
        request->set_payload(payload);
        app->send(request);
        std::cout << "sent message: \"" << mydata << "\"\n";
    }

    app->clear_all_handler();
    app->release_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    condition.notify_one();
    app->stop();
}

void on_message(const std::shared_ptr<vsomeip::message> &_response) {

  std::shared_ptr<vsomeip::payload> its_payload = _response->get_payload();
  vsomeip::length_t l = its_payload->get_length();

  // Get payload
  std::stringstream ss;
  for (vsomeip::length_t i=0; i<l; i++) {
     ss << std::setw(2) << std::setfill('0') << std::hex
        << (int)*(its_payload->get_data()+i) << " ";
  }

  std::cout << "CLIENT: Received message with Client/Session ["
      << std::setw(4) << std::setfill('0') << std::hex << _response->get_client() << "/"
      << std::setw(4) << std::setfill('0') << std::hex << _response->get_session() << "] "
      << ss.str() << std::endl;
}

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << "CLIENT: Service ["
            << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
            << "] is "
            << (_is_available ? "available." : "NOT available.")
            << std::endl;
    condition.notify_one();
}

int main() {

    app = vsomeip::runtime::get()->create_application("someip-fw");
    app->init();
    app->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, on_availability);
    app->request_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    app->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID, on_message);
    std::thread sender(run);
    app->start();

    sender.join();
}