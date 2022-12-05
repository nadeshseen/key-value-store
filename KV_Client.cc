#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "KV_Store.grpc.pb.h"

//IO
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <climits>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using kv_store::KeyValueService;
using kv_store::KV_Reply;
using kv_store::KV_Request;
// using kv_store::GET_KV_Request;
// using kv_store::PUT_KV_Request;
// using kv_store::DEL_KV_Request;
float min_resp_time = INT_MAX, max_resp_time = 0, sum_resp_time = 0, avg_resp_time;
int request_count = 0;
std::string default_ip = "0.0.0.0:";
class KeyValueServiceClient
{
public:
  KeyValueServiceClient(std::shared_ptr<Channel> channel) : stub_(KeyValueService::NewStub(channel)) {}

  std::string GENERAL_KV(const std::string &KEY, const std::string &VALUE, const std::string &TYPE, const std::string &ID, const std::string &PORT, const std::string &PATH)
  {
    // Follows the same pattern as SayHello.
    KV_Request request;
    request.set_key(KEY);
    request.set_value(VALUE);
    request.set_type(TYPE);
    request.set_client_id(ID);
    request.set_port_no(PORT);
    request.set_path(PATH);
    KV_Reply reply;
    ClientContext context;

    // Here we can use the stub's newly available method we just added.
    Status status = stub_->GENERAL_KV(&context, request, &reply);
    if (status.ok())
    {
      if (reply.status_code() == "400")
      {
        return reply.message();
      }
      return reply.message();
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

private:
  std::unique_ptr<KeyValueService::Stub> stub_;
};

int main(int argc, char **argv)
{

  // Input from CONFIG file
  std::ifstream config_file("../Input/CONFIG");
  std::string line;

  int CACHE_SIZE;
  std::string CACHE_REPLACEMENT_TYPE;
  std::string port_no;
  std::string mode;
  while (std::getline(config_file, line))
  {
    std::string chunk;
    std::stringstream ss(line);
    std::vector<std::string> tokenize;

    while (ss >> chunk)
      tokenize.push_back(chunk);
    // std::cout<<tokenize[0]<<" "<<tokenize[1]<<std::endl;
    if (tokenize[0] == "LISTENING_PORT")
      port_no = tokenize[1];
    else if (tokenize[0] == "MODE")
      mode = tokenize[1];
  }
  config_file.close();
  if (argc > 1)
  {
    port_no = argv[1];
    std::cout << port_no << std::endl;
  }
  std::string target_str;
  target_str = default_ip + port_no;

  KeyValueServiceClient client(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  //Current time since epoch 1634883999999
  auto time_since = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  std::string curr_time = std::to_string(time_since % 1000000);
  std::string file_name = "../Output/" + curr_time + "_client_log.txt";
  std::ofstream client_log(file_name);
  // open = batch_file

  if (mode == "BATCH")
  {
    std::ifstream batch_file("../Input/batch.txt");
    line = "";

    std::string key, value, type, client_id, port_no;
    key = "";
    value = "";
    type = "START";
    client_id = curr_time;
    std::string start_client = client.GENERAL_KV(key, value, type, client_id, port_no, "");
    while (std::getline(batch_file, line))
    {
      std::string chunk;
      std::stringstream ss(line);
      std::vector<std::string> tokenize;
      while (ss >> chunk)
        tokenize.push_back(chunk);
      key = "";
      value = "";
      type = "";
      if (tokenize[0] == "PUT")
      {
        std::cout << "PUT Execution" << std::endl;

        key = tokenize[1];
        if (tokenize.size() == 3)
          value = tokenize[2];
        type = "PUT";
        auto start = std::chrono::high_resolution_clock::now();
        std::string put_reply = client.GENERAL_KV(key, value, type, client_id, port_no, "");
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        int dur = duration.count();
        // std::cout<<"Response Time for PUT is  "<<dur<<" microseconds"<<std::endl;
        //Open client log file in append mode
        // std:: ofstream client_log("../Output/client_log.txt",std::ios::app);
        std::string dur_str = std::to_string(dur);
        std::string m = "Response time for PUT is " + dur_str + " microseconds\n";
        client_log << m;

        //Collecting Statistics for Response Time
        if (dur < min_resp_time)
          min_resp_time = dur;
        if (dur > max_resp_time)
          max_resp_time = dur;
        sum_resp_time += dur;
        request_count++;

        std::cout << "Client received: " << put_reply << std::endl;
        // std::cout<<"################################################"<<std::endl;
        std::cout << std::endl;
      }
      else if (tokenize[0] == "GET")
      {
        std::cout << "GET Execution" << std::endl;
        key = tokenize[1];
        value = "";
        type = "GET";
        auto start = std::chrono::high_resolution_clock::now();
        std::string get_reply = client.GENERAL_KV(key, value, type, client_id, port_no, "");
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        int dur = duration.count();
        // std::cout<<"Response Time for GET is  "<<dur<<" microseconds"<<std::endl;
        //Open client log file in append mode
        // std:: ofstream client_log("../Output/client_log.txt",std::ios::app);
        std::string dur_str = std::to_string(dur);
        std::string m = "Response time for GET is " + dur_str + " microseconds\n";
        client_log << m;

        //Collecting Statistics for Response Time
        if (dur < min_resp_time)
          min_resp_time = dur;
        if (dur > max_resp_time)
          max_resp_time = dur;
        sum_resp_time += dur;
        request_count++;

        std::cout << "Client received: " << get_reply << std::endl;
        //cout<<result+" for key "+key<<endl;
        // std::cout<<"################################################"<<std::endl;
        std::cout << std::endl;
      }
      else if (tokenize[0] == "DEL")
      {
        std::cout << "DEL Execution" << std::endl;
        key = tokenize[1];
        value = "";
        type = "DEL";
        auto start = std::chrono::high_resolution_clock::now();
        std::string del_reply = client.GENERAL_KV(key, value, type, client_id, port_no, "");
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        int dur = duration.count();
        // std::cout<<"Response Time for DEL is  "<<dur<<" microseconds"<<std::endl;
        // Open client log file in append mode
        // std:: ofstream client_log("../Output/client_log.txt",std::ios::app);
        std::string dur_str = std::to_string(dur);
        std::string m = "Response time for DEL is " + dur_str + " microseconds\n";
        client_log << m;

        //Collecting Statistics for Response Time
        if (dur < min_resp_time)
          min_resp_time = dur;
        if (dur > max_resp_time)
          max_resp_time = dur;
        sum_resp_time += dur;
        request_count++;
        std::cout << "Client received: " << del_reply << std::endl;
        // std::cout<<"################################################"<<std::endl;
        std::cout << std::endl;
      }
    }
    key = "";
    value = "";
    type = "";
    type = "END";
    std::string end_client = client.GENERAL_KV(key, value, type, client_id, port_no, "");
    client_log.close();
    batch_file.close();
  }

  else if (mode == "INTERACTIVE")
  {

    std::cout << "You are in interactive mode\n"
              << std::endl;
    std::cout << "You can issue 3 formats of requests:\n 1) GET <key>\n 2) PUT <key> <value>\n 3) DEL <key>" << std::endl;
    std::cout << "To exit interactive mode, type EXIT or exit and press enter" << std::endl;
    std::string key, value, type, client_id, port_no;
    key = "";
    value = "";
    type = "START";
    client_id = curr_time;
    std::string start_client = client.GENERAL_KV(key, value, type, client_id, port_no, "");
    while (1)
    {
      std::string line;
      std::string chunk;
      key = "";
      value = "";
      type = "";
      client_id = curr_time;
      getline(std::cin, line);
      if (line == "exit" || line == "EXIT")
        break;
      std::stringstream ss(line);
      std::vector<std::string> tokenize;
      while (ss >> chunk)
        tokenize.push_back(chunk);
      if (tokenize[0] == "PUT" || tokenize[0] == "put")
      {
        std::cout << "PUT Execution" << std::endl;

        key = tokenize[1];
        value = tokenize[2];
        type = "PUT";
        auto start = std::chrono::high_resolution_clock::now();
        std::string put_reply = client.GENERAL_KV(key, value, type, client_id, port_no, "");
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        int dur = duration.count();
        // std::cout<<"Response Time for PUT is  "<<dur<<" microseconds"<<std::endl;
        //Open client log file in append mode
        // std:: ofstream client_log("../Output/client_log.txt",std::ios::app);
        std::string dur_str = std::to_string(dur);
        std::string m = "Response time for PUT is " + dur_str + " microseconds\n";
        client_log << m;

        //Collecting Statistics for Response Time
        if (dur < min_resp_time)
          min_resp_time = dur;
        if (dur > max_resp_time)
          max_resp_time = dur;
        sum_resp_time += dur;
        request_count++;

        std::cout << "Client received: " << put_reply << std::endl;
        // std::cout<<"################################################"<<std::endl;
        std::cout << std::endl;
      }

      else if (tokenize[0] == "GET" || tokenize[0] == "get")
      {
        std::cout << "GET Execution" << std::endl;
        key = tokenize[1];
        value = "";
        type = "GET";
        auto start = std::chrono::high_resolution_clock::now();
        std::string get_reply = client.GENERAL_KV(key, value, type, client_id, port_no, "");
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        int dur = duration.count();
        // std::cout<<"Response Time for GET is  "<<dur<<" microseconds"<<std::endl;
        //Open client log file in append mode
        // std:: ofstream client_log("../Output/client_log.txt",std::ios::app);
        std::string dur_str = std::to_string(dur);
        std::string m = "Response time for GET is " + dur_str + " microseconds\n";
        client_log << m;

        //Collecting Statistics for Response Time
        if (dur < min_resp_time)
          min_resp_time = dur;
        if (dur > max_resp_time)
          max_resp_time = dur;
        sum_resp_time += dur;
        request_count++;

        std::cout << "Client received: " << get_reply << std::endl;
        //cout<<result+" for key "+key<<endl;
        // std::cout<<"################################################"<<std::endl;
        std::cout << std::endl;
      }

      else if (tokenize[0] == "DEL" || tokenize[0] == "del")

      {
        std::cout << "DEL Execution" << std::endl;
        key = tokenize[1];
        value = "";
        type = "DEL";
        auto start = std::chrono::high_resolution_clock::now();
        std::string del_reply = client.GENERAL_KV(key, value, type, client_id, port_no, "");
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        int dur = duration.count();
        // std::cout<<"Response Time for DEL is  "<<dur<<" microseconds"<<std::endl;
        // Open client log file in append mode
        // std:: ofstream client_log("../Output/client_log.txt",std::ios::app);
        std::string dur_str = std::to_string(dur);
        std::string m = "Response time for DEL is " + dur_str + " microseconds\n";
        client_log << m;

        //Collecting Statistics for Response Time
        if (dur < min_resp_time)
          min_resp_time = dur;
        if (dur > max_resp_time)
          max_resp_time = dur;
        sum_resp_time += dur;
        request_count++;
        std::cout << "Client received: " << del_reply << std::endl;
        // std::cout<<"################################################"<<std::endl;
        std::cout << std::endl;
      }
      else
        std::cout << "Invalid format. Make sure GET,PUT,DEL and key/value are alll separated by a single space only!!" << std::endl;
    }
    key = "";
    value = "";
    type = "END";
    client_id = curr_time;
    std::string end_client = client.GENERAL_KV(key, value, type, client_id, port_no, "");
    client_log.close();
  }

  //Open in trunc mode to clear previous entries in the file
  std::ofstream client_stat("../Output/" + curr_time + "_client_stat.txt", std::ios::trunc);

  //Write average statistics in client stats file for 1 batch iteration
  std::string min_message = "Min Response time is " + std::to_string(min_resp_time) + " microseconds\n";
  client_stat << min_message;

  std::string max_message = "Max Response time is " + std::to_string(max_resp_time) + " microseconds\n";
  client_stat << max_message;

  avg_resp_time = sum_resp_time / request_count;
  std::string avg_message = "Avg Response time is " + std::to_string(avg_resp_time) + " microseconds\n";
  client_stat << avg_message;

  std::string sum_message = "Sum of all Response time is " + std::to_string(sum_resp_time) + " microseconds\n";
  client_stat << sum_message;

  std::string count_message = "Total requests received are " + std::to_string(request_count) + "\n";
  client_stat << count_message;
  client_stat.close();
  return 0;
}
