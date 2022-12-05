#include <iostream>
#include <memory>
#include <string>
#include <vector>
//Including threading library
// #include <pthread.h>

//Including C++ threading lib
#include <thread>

#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <dirent.h>
#include <openssl/sha.h>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include "KV_Store.grpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

// Manual Client code on server
using grpc::Channel;
using grpc::ClientContext;

using kv_store::Join_Request;
using kv_store::KeyValueService;
using kv_store::KV_Reply;
using kv_store::KV_Request;

// using kv_store::GET_KV_Request;
// using kv_store::PUT_KV_Request;
// using kv_store::DEL_KV_Request;

using namespace std;

ofstream log_file;

int put_key_val(string key, string value);
string del_key(string key);
string get_key(string key);

typedef struct key_value_entry
{
  string key;
  string value;
  // int dirty;
} KeyValueEntry;

vector<pair<string, int>> lru;
unordered_map<string, KeyValueEntry> Cache;
enum REQUEST_RESPONSE
{
  GET_ST = 1,
  PUT_ST = 2,
  DEL_ST = 3,
  SUCCESS_STATUS_CODE = 200,
  ERROR_STATUS_CODE = 400
};
string default_ip="0.0.0.0:";
string port_no;
int id;
string successor_port;
string successor_id;
string predecessor_port;
string predecessor_id;
string list_of_servers;

// typedef struct finger_table{
//   string start;
//   string successor_id;
//   string successor_port;
// }FingerTable;

struct finger_entry
{
  string start;
  string end;
  string successor_id;
  string successor_port;
};

// FingerTable FT[16];
vector<finger_entry> FT(16);
vector<pair<int, string>> vector_list;

int CLIENT_TYPE = 0, JOIN_TYPE = 1;

string CACHE_POLICY;
int CACHE_SIZE;
int first_client = 1;
std::chrono::time_point<std::chrono::high_resolution_clock> client_start_time;
std::chrono::time_point<std::chrono::high_resolution_clock> client_end_time;

pthread_rwlock_t lock_cache;
pthread_rwlock_t lock_log_file;
pthread_rwlock_t lockFile[128];

// Manual Client code on server
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
  KV_Reply Server_Join(const std::string &MY_PORT, const std::string &MY_ID, const std::string &TARGET_PORT, const std::string &TYPE, const std::string &LIST)
  {
    // Follows the same pattern as SayHello.
    Join_Request request;
    request.set_port(MY_PORT);
    request.set_id(MY_ID);
    request.set_target_port(TARGET_PORT);
    request.set_type(TYPE);
    request.set_list(LIST);
    // Add message in the request

    KV_Reply reply;
    ClientContext context;

    // Here we can use the stub's newly available method we just added.
    Status status = stub_->Server_Join(&context, request, &reply);
    if (status.ok())
    {
      return reply;
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return reply;
    }
  }

private:
  std::unique_ptr<KeyValueService::Stub> stub_;
};

bool is_invalid(int size, string text, string &server_stat_code, string &server_reply)
{
  if (size > 250)
  {
    server_reply = "Error : " + text + " size greater than 256 bytes";
  }
  else
  {
    return false;
  }
  server_stat_code = "400";
  return true;
}



int hash_string(string key)
{
  int hash_byte = 7;
  int i = 0;
  unsigned char temp[hash_byte];
  char buf[hash_byte * 2];

  memset(buf, 0x0, hash_byte * 2);
  memset(temp, 0x0, hash_byte);

  char key_arr[key.length()];
  strcpy(key_arr, key.c_str());

  SHA1((unsigned char *)key_arr, strlen(key_arr), temp);

  for (i = 0; i < hash_byte; i++)
  {
    sprintf((char *)&(buf[i * 2]), "%02x", temp[i]);
  }

  //printf("SHA1 of %s is %s\n", key_arr, buf);

  char hash_arr[4];
  for (int i = 0; i < 4; i++)
  {
    hash_arr[i] = buf[i];
  }

  string hash_string = hash_arr;

  int hash_val = std::stoi(hash_string, 0, 16);

  //change only this for testing
  // hash_val = stoi(key);
  return hash_val;
}
void find_next_hop(int &hashed_key, string &next_hop_id, string &next_hop_port)
{
  for (int i = 15; i >= 0; i--)
  {

    int self_check = 1;
    if (stoi(FT[i].end) >= stoi(FT[i].start))
    {
      if (hashed_key <= stoi(FT[i].end) && hashed_key >= stoi(FT[i].start))
      {
        self_check = 1;
      }
      else
      {
        self_check = 0;
      }
    }
    else
    {
      if ((hashed_key <= stoi(FT[i].end) && hashed_key >= 0) || (hashed_key >= stoi(FT[i].start) && hashed_key < pow(2, 16)))
      {
        self_check = 1;
      }
      else
      {
        self_check = 0;
      }
    }

    if (self_check == 1)
    {
      next_hop_id = FT[i].successor_id;
      next_hop_port = FT[i].successor_port;
      return;
    }
  }
}

void find_successor(int &key_id, string &succ_port, string &succ_id)
{
  for (int i = 0; i < vector_list.size(); i++)
  {
    if (key_id <= vector_list[i].first)
    {
      succ_id = to_string(vector_list[i].first);
      succ_port = vector_list[i].second;
      return;
    }
  }

  succ_id = to_string(vector_list[0].first);
  succ_port = vector_list[0].second;
}
vector<pair<string, string>> transfer_key_val(string port, string file_name, string new_file_name, int pred)
{
  //To store key value pair
  vector<pair<string, string>> key_val;
  //Find index of start and end portion of string to capture
  size_t uscore = file_name.find("_"), txt = file_name.find(".txt");

  if (txt > uscore) //Valid Filename
  {
    string to_compare = "";
    for (int i = uscore + 1; i < txt; i++)
    {
      to_compare += file_name[i];
    }
    string start_file = "";
    for (int i = 0; i < uscore; i++)
    {
      start_file += file_name[i];
    }
    // cout << "start" << start_file << endl;
    //if(port==to_compare) cout<<"Match!!"<<endl; else cout<<"Unequal"<<endl;
    if (port == to_compare)
    {

      fstream file("../Output/KV_files/" + file_name, ios::in | ios::out);
      if (file.is_open())
      {

        ofstream new_server("../Output/KV_files/" + start_file + "_" + new_file_name + ".txt", ios::app);
        char buffer[514];
        while (file.read(buffer, 514))
        {
          //Extract key and value for live keys
          if (buffer[512] == '1') //Means live
          {
            string extract_key = buffer, extract_val = buffer + 256;
            //Nadesh to Insert logic to compare extract_key hashed value to some range

            int hashed_key = hash_string(extract_key);
            // cout<<"Hashed key when transfering"<<endl;
            int self_check = 1;

            int curr = id;
            if (curr > pred)
            {
              if (hashed_key <= curr && hashed_key > pred)
              {
                self_check = 1;
              }
              else
              {
                self_check = 0;
              }
            }
            else
            {
              if ((hashed_key <= curr && hashed_key >= 0) || (hashed_key >= pred && hashed_key < pow(2, 16)))
              {
                self_check = 1;
              }
              else
              {
                self_check = 0;
              }
            }
            //Current always true
            if (self_check == 0)
            {
              new_server.write(buffer, 514);
              //Change live key to DEAD since it is transferred to new server
              file.seekp(-2, ios::cur);
              file.write("0\n", 2);

              //Store extracted key and value in a vector
              key_val.push_back(make_pair(extract_key, extract_val));
              //for (int i=0;i<extract_key.size();i++) cout<<extract_key[i];
              //cout<<""<<endl;
              //for (int i=0;i<extract_val.size();i++) cout<<extract_val[i];
              //cout<<""<<endl;
            }
          }
        }
        new_server.close();
      }
    }
  }
  return key_val;
}
void initialize_ft()
{
  int mod_val = pow(2, 16);
  for (int i = 0; i < 16; i++)
  {
    int st = id + pow(2, i);
    st = st % mod_val;
    FT[i].start = to_string(st);
    int end = id + pow(2, i + 1) - 1;
    end = end % mod_val;
    FT[i].end = to_string(end);
    find_successor(st, FT[i].successor_port, FT[i].successor_id);
  }
}

void print_fingertable()
{

  // cout<<setfill('-')<<"|"<<setw(10)<<"-"<<"|"<<setw(10)<<"-"<<"|"<<setw(10)<<"-"<<"|"<<setw(10)<<"-"<<"|"<<endl;
  cout << setfill(' ') << "|" << setw(10) << "i"
       << "|" << setw(10) << "Start"
       << "|" << setw(10) << "ID"
       << "|" << setw(15) << "IP"
       << "|" << endl;
  cout << setfill('-') << "|" << setw(10) << "-"

       << "|" << setw(10) << "-"
       << "|" << setw(10) << "-"
       << "|" << setw(15) << "-"
       << "|" << endl;
  for (int i = 0; i < 16; i++)
  {
    cout << setfill(' ') << "|" << setw(10) << i << "|" << setw(10) << FT[i].start << "|" << setw(10) << FT[i].successor_id << "|" << setw(15) << FT[i].successor_port << "|" << endl;
    // cout<<setfill('-')<<"|"<<setw(10)<<"-"<<"|"<<setw(10)<<"-"<<"|"<<setw(10)<<"-"<<"|"<<setw(10)<<"-"<<"|"<<endl;
  }
}

void update_succ_pred()
{
  int n = vector_list.size();
  for (int i = 0; i < n; i++)
  {
    if (vector_list[i].first == id)
    {
      if (i == 0)
      {
        predecessor_id = to_string(vector_list[n - 1].first);
        predecessor_port = vector_list[n - 1].second;
        successor_id = to_string(vector_list[i + 1].first);
        successor_port = vector_list[i + 1].second;
      }
      else if (i == n - 1)
      {
        predecessor_id = to_string(vector_list[i - 1].first);
        predecessor_port = vector_list[i - 1].second;
        successor_id = to_string(vector_list[0].first);
        successor_port = vector_list[0].second;
      }
      else
      {
        predecessor_id = to_string(vector_list[i - 1].first);
        predecessor_port = vector_list[i - 1].second;
        successor_id = to_string(vector_list[i + 1].first);
        successor_port = vector_list[i + 1].second;
      }
    }
  }
}
vector<string> tokenize(string s, string del)
{
  vector<string> v;
  int start = 0;
  int end = s.find(del);
  while (end != -1)
  {
    v.push_back(s.substr(start, end - start));
    start = end + del.size();
    end = s.find(del, start);
  }
  v.push_back(s.substr(start, end - start));
  return v;
}

vector<pair<int, string>> separated_id_port(string list)
{
  vector<string> ip_port = tokenize(list, ",");

  //Couple stores pair of (int:server_id,string:server_id-portno)
  vector<pair<int, string>> couple;
  for (int i = 0; i < ip_port.size(); i++)
  {
    vector<string> v = tokenize(ip_port[i], "-");
    int id = stoi(v[0]);
    couple.push_back(make_pair(id, v[1]));
  }
  return couple;
}

string sort_list(string list)
{
  // Takes C++ string with any separator
  // string list = "5-6000,1-6001,3-6002,2-7089";
  //Split on comma
  vector<string> ip_port = tokenize(list, ",");

  //Couple stores pair of (int:server_id,string:server_id-portno)
  vector<pair<int, string>> couple;
  for (int i = 0; i < ip_port.size(); i++)
  {
    vector<string> v = tokenize(ip_port[i], "-");
    int ip = stoi(v[0]);
    couple.push_back(make_pair(ip, ip_port[i]));
  }
  //Sorting the couple.
  //To access the ith element of couple, use: couple[i].first for server_id and couple[i].second for server_id-portno
  sort(couple.begin(), couple.end());

  //String to store sorted list
  string sorted_list = "";
  for (int i = 0; i < ip_port.size(); i++)
  {
    if (i < ip_port.size() - 1)
    {
      sorted_list += couple[i].second + ",";
    }
    else
    {
      sorted_list += couple[i].second;
    }
  }
  // cout << sorted_list << endl;
  return sorted_list;
}

class ServerImpl final
{
public:
  ~ServerImpl()
  {
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    for (int i = 0; i < num_threads; i++)
    {
      list_of_cqs_[i]->Shutdown();
    }
  }

  // There is no shutdown handling in this code.
  void Run()
  {

    //Input from CONFIG file

    std::ifstream config_file("../Input/CONFIG");

    std::string line;

    // std::string port_no;
    std::string CACHE_REPLACEMENT_TYPE;
    while (std::getline(config_file, line))
    {
      std::string chunk;
      std::stringstream ss(line);
      std::vector<std::string> tokenize;

      while (ss >> chunk)
        tokenize.push_back(chunk);
      // std::cout << tokenize[0] << " " << tokenize[1] << std::endl;
      if (tokenize[0] == "LISTENING_PORT" && port_no.size() == 0)
        port_no = tokenize[1];
      else if (tokenize[0] == "CACHE_REPLACEMENT_TYPE")
        CACHE_POLICY = tokenize[1];
      else if (tokenize[0] == "CACHE_SIZE")
        CACHE_SIZE = std::stoi(tokenize[1]);
      else if (tokenize[0] == "THREAD_POOL_SIZE")
        num_threads = std::stoi(tokenize[1]);
    }
    cout << endl;
    config_file.close();
    std::string server_address = default_ip + port_no;
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    // Register "service_" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *asynchronous* service.
    builder.RegisterService(&service_);
    // Get hold of the completion queue used for the asynchronous communication
    // with the gRPC runtime.

    for (int i = 0; i < num_threads; i++)
    {
      list_of_cqs_.push_back(builder.AddCompletionQueue());
    }

    // Finally assemble the server.
    server_ = builder.BuildAndStart();
    // std::cout << "Server listening on " << server_address << std::endl;

    // Initial server in message

    //Current time snipet
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
    auto time_now = oss.str();
    log_file.open("../Output/server_log_" + port_no + ".txt");
    log_file << time_now << " : Server started successfully on " << server_address << endl
             << endl;

    // Using thread
    std::thread th[num_threads];
    for (int i = 0; i < num_threads; i++)
    {
      th[i] = std::thread(&ServerImpl::HandleRpcs, this, i);
    }
    for (int i = 0; i < num_threads; i++)
    {
      th[i].join();
    }
  }

private:
  // Class encompasing the state and logic needed to serve a request.
  class CallData
  {
  public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    CallData(KeyValueService::AsyncService *service, ServerCompletionQueue *cq, int value)
        : service_(service), calldata_cq_(cq), responder_(&ctx_), status_(CREATE)
    {
      // Invoke the serving logic right away.
      REQUEST_TYPE = value;
      // std::cout<<"Completion Queue - "<<calldata_cq_<<std::endl;
      Proceed(-1);
    }

    void Proceed(int server_id)
    {
      if (status_ == CREATE)
      {
        // Make this instance progress to the PROCESS state.
        status_ = PROCESS;
        // As part of the initial CREATE state, we *request* that the system
        // start processing SayHello requests. In this request, "this" acts are
        // the tag uniquely identifying the request (so that different CallData
        // instances can serve different requests concurrently), in this case
        // the memory address of this CallData instance.
        if (REQUEST_TYPE == CLIENT_TYPE)
        {
          service_->RequestGENERAL_KV(&ctx_, &request_, &responder_, calldata_cq_, calldata_cq_, this);
          // cout<<"Will this work"<<endl;
        }
        else if (REQUEST_TYPE == JOIN_TYPE)
        {
          service_->RequestServer_Join(&ctx_, &join_request_, &responder_, calldata_cq_, calldata_cq_, this);
        }
      }
      else if (status_ == PROCESS)
      {
        // Spawn a new CallData instance to serve new clients while we process
        // the one for this CallData. The instance will deallocate itself as
        // part of its FINISH state.
        new CallData(service_, calldata_cq_, REQUEST_TYPE);

        // int time = 1;
        // while(time){
        //   sleep(1);
        //   std::cout<<time<<" time left on server "<<server_id<<std::endl;
        //   time--;
        // }

        if (REQUEST_TYPE == JOIN_TYPE)
        {
          // cout << "Hello " << join_request_.port() << " " << join_request_.id() << " my name is " << port_no << " " << id << endl;

          if (join_request_.type() == "NewServer")
          {

            reply_.set_message("Server with id = " + to_string(id) + " is sending this reply");
            reply_.set_id(to_string(id));
            reply_.set_successor_id(successor_id);
            reply_.set_successor_port(successor_port);

            list_of_servers += "," + join_request_.id() + "-" + join_request_.port();

            list_of_servers = sort_list(list_of_servers);
            vector_list = separated_id_port(list_of_servers);
            update_succ_pred();

            reply_.set_list(list_of_servers);

            // cout << "Sending broadcast message every server except " << id << " and " << join_request_.id() << endl;
            for (int i = 0; i < vector_list.size(); i++)
            {
              // cout<<vector_list[i].first<<" "<<vector_list[i].second<<endl;
              if (vector_list[i].first != id && vector_list[i].first != stoi(join_request_.id()))
              {
                string target_str = default_ip + vector_list[i].second;
                // cout<<target_str<<endl;
                KeyValueServiceClient server_as_client = grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials());
                KV_Reply server_bhayia = server_as_client.Server_Join("", "", "", "Update", list_of_servers);
              }
            }
          }
          else if (join_request_.type() == "Update")
          {
            // cout << "Updating Server with id = " << id << endl;
            list_of_servers = join_request_.list();

            vector_list = separated_id_port(list_of_servers);
            // Convert logic
            update_succ_pred();
          }
          else if (join_request_.type() == "KeyTransfer")
          {
            Cache.clear();
            lru.clear();

            DIR *dir;
            struct dirent *diread;
            vector<string> files;

            if ((dir = opendir("../Output/KV_files")) != nullptr)
            {
              while ((diread = readdir(dir)) != nullptr)
              {
                files.push_back(diread->d_name);
              }
              closedir(dir);
            }
            // cout<<"Nadesh"<<endl;
            //  else {
            //     perror ("opendir");
            //     return EXIT_FAILURE;
            // }

            string new_file_ending = join_request_.port();
            int pred = stoi(join_request_.id());
            for (auto file_str : files)
            {
              string port = port_no;
              string file = file_str;
              vector<pair<string, string>> key_val = transfer_key_val(port, file, new_file_ending, pred);

              // cout<<"Checking for "+file<<endl;
              // if (key_val.size()>0) //Port number found within file name
              // {
              //   for (int i=0;i<key_val.size();i++)
              //   {
              //       string key=key_val[i].first;
              //       string value=key_val[i].second;
              //       cout<<key+" "+value<<endl;
              //   }
              // }
              // else cout<<"Match not found!!"<<endl;
            }
          }

          // cout << "Successor - " << successor_port << " " << successor_id << endl;
          // cout << "Predecessor - " << predecessor_port << " " << predecessor_id << endl;
          // cout << "List - " << list_of_servers << endl;
          initialize_ft();
          cout << "Server (" << id << ", " << port_no << ")" << endl;
          print_fingertable();
        }
        else if (REQUEST_TYPE == CLIENT_TYPE)
        {

          string key = request_.key();
          string value = request_.value();

          string server_reply;
          string server_stat_code;
          string client_log_string;

          // cout<<request_.type()<<endl;

          auto t = std::time(nullptr);
          auto tm = *std::localtime(&t);
          std::ostringstream oss;
          oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
          auto time_now = oss.str();
          client_log_string += time_now + " : \n";

          bool is_key_invalid = is_invalid(key.size(), "Key", server_stat_code, server_reply);

          if (request_.type() == "START")
          {
            client_log_string += "Server " + to_string(server_id) + " accepts client " + request_.client_id() + " connection" + "\n";
            if (first_client == 1)
            {
              client_start_time = std::chrono::high_resolution_clock::now();
              first_client = 0;
            }
          }
          else if (request_.type() == "END")
          {
            client_log_string += "Server " + to_string(server_id) + " ends client " + request_.client_id() + " connection" + "\n";

            // Total Response time logic

            // client_end_time = std::chrono::high_resolution_clock::now();
            // auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (client_end_time-client_start_time);
            // int dur=duration.count();
            // ofstream total_time("../Output/Total_time.txt");
            // total_time<<dur<<endl;
            // total_time.close();
          }
          else
          {
            if (is_key_invalid)
            {
              client_log_string += "Client-" + request_.client_id() + " : Request[\"" + request_.type();
              client_log_string += "\", Parameters-(Key:\"Invalid\")]\n";
            }
            else
            {

              int key_id = hash_string(key);
              cout << key << " - " << key_id << endl;
              client_log_string += "Client-" + request_.client_id() + " : Request[\"" + request_.type();

              client_log_string += "\", Parameters-(Key:\"" + key;
              int invalid_val_flag = 0;
              if (request_.type() == "PUT")
              {
                if (is_invalid(value.size(), "Value", server_stat_code, server_reply))
                {
                  invalid_val_flag = 1;
                  client_log_string += "\", Value:\"Invalid";
                }
                else
                {
                  client_log_string += "\", Value:\"" + value;
                }
              }
              client_log_string += "\")]\n";
              string next_hop_id, next_hop_port;
              if (invalid_val_flag == 0)
              {
                int self_check = 1;
                if (id > stoi(predecessor_id))
                {
                  if (key_id <= id && key_id > stoi(predecessor_id))
                  {
                    self_check = 1;
                  }
                  else
                  {
                    self_check = 0;
                  }
                }
                else
                {
                  if ((key_id <= id && key_id >= 0) || (key_id >= stoi(predecessor_id) && key_id < pow(2, 16)))
                  {
                    self_check = 1;
                  }
                  else
                  {
                    self_check = 0;
                  }
                }

                if (self_check == 1)
                {
                  cout << "Keys maps to this server "<<id<<" "<<port_no << endl;

                  if (request_.type() == "GET")
                  {

                    pthread_rwlock_rdlock(&lock_cache);
                    bool key_not_present = Cache.find(key) == Cache.end();
                    pthread_rwlock_unlock(&lock_cache);
                    if (key_not_present)
                    {
                      // cout << key<<" Element Not Present in cache\n"; //search in storage and replacement in cache
                      string value = get_key(key); //return from PSTORAGE
                      if (value == "NULL")
                      {
                        server_stat_code = "400";
                        server_reply = "Error : (Key-\"" + key + "\") Not Exists";
                      }
                      else
                      {
                        server_stat_code = "200";
                        server_reply = "(Key-\"" + key + "\") found in disk and returned (Value-\"" + value + "\")";
                        pthread_rwlock_rdlock(&lock_cache);
                        bool is_cache_full = lru.size() >= CACHE_SIZE;
                        pthread_rwlock_unlock(&lock_cache);

                        if (is_cache_full)
                        {
                          //replacement in cache
                          //last entry is least recently used**
                          //check for isvalid bit and copy back to storage
                          pthread_rwlock_wrlock(&lock_cache);

                          if (CACHE_POLICY == "LRU")
                          {
                            // std::cout<<"LRU"<<std::endl;
                            Cache.erase(lru[0].first);
                            for (int i = 0; i < lru.size() - 1; i++)
                            {
                              lru[i] = lru[i + 1];
                            }
                          }
                          else
                          {
                            // std::cout<<"LFU"<<std::endl;
                            int min = 0;
                            for (int i = 1; i < lru.size(); i++)
                            {
                              if (lru[min].second > lru[i].second)
                                min = i;
                            }
                            Cache.erase(lru[min].first);
                            for (int i = min; i < lru.size() - 1; i++)
                            {
                              lru[i] = lru[i + 1];
                            }
                          }
                          lru.pop_back();
                          pthread_rwlock_unlock(&lock_cache);
                        }
                        // Cache[key]=key;
                        KeyValueEntry k1;
                        //from storage
                        k1.key = key;
                        k1.value = value;
                        pthread_rwlock_wrlock(&lock_cache);
                        Cache.insert(make_pair(key, k1));
                        lru.push_back(make_pair(key, 1));
                        pthread_rwlock_unlock(&lock_cache);
                      }
                    }
                    else
                    {
                      server_stat_code = "200";
                      pthread_rwlock_rdlock(&lock_cache);
                      server_reply = "(Key-\"" + key + "\") found in Cache and returned (Value-\"" + Cache[key].value + "\")";
                      pthread_rwlock_unlock(&lock_cache);
                      int toupdate = 0;

                      pthread_rwlock_wrlock(&lock_cache);
                      for (int i = 0; i < lru.size(); i++)
                      {
                        if (string(lru[i].first) == key)
                        {
                          toupdate = i;
                          lru[i].second += 1;
                          break;
                        }
                      }
                      pair<string, int> temp = lru[toupdate];
                      for (int i = toupdate; i < lru.size() - 1; i++)
                      {
                        lru[i] = lru[i + 1];
                      }
                      // lru[0]=temp;
                      lru[lru.size() - 1] = temp;
                      pthread_rwlock_unlock(&lock_cache);

                      char first_char = key[0];
                      int ascii = first_char;
                      string ascii_str = to_string(ascii);
                      // ifstream file ("../Output/KV_files/"+ascii_str+".txt");
                      // ofstream client_log_string+=("../Output/logfile.txt",ios::app);
                      // client_log_string+=<<"Extracted value for key "+key+" is "+Cache[key].value<<endl;;
                      // cout<<"updated lru";
                    }
                  }
                  else if (request_.type() == "PUT")
                  {
                    if (is_invalid(value.size(), "Value", server_stat_code, server_reply))
                    {
                      client_log_string += "\", Parameters-(Value:\"Invalid\")]\n";
                    }
                    else
                    {

                      pthread_rwlock_rdlock(&lock_cache);
                      bool key_not_present = Cache.find(key) == Cache.end();
                      pthread_rwlock_unlock(&lock_cache);
                      if (key_not_present)
                      {
                        // cout <<  key<<" Element Not Present in cache\n"; //search in storage and replacement in cache
                        put_key_val(key, value);

                        pthread_rwlock_rdlock(&lock_cache);
                        bool is_cache_full = lru.size() >= CACHE_SIZE;
                        pthread_rwlock_unlock(&lock_cache);

                        if (is_cache_full)
                        {

                          //replacement in cache
                          //last entry is least recently used**
                          //check for isvalid bit and copy back to storage

                          pthread_rwlock_wrlock(&lock_cache);
                          if (CACHE_POLICY == "LRU")
                          {
                            // std::cout<<"LRU"<<std::endl;
                            Cache.erase(lru[0].first);
                            for (int i = 0; i < lru.size() - 1; i++)
                            {
                              lru[i] = lru[i + 1];
                            }
                          }
                          else
                          {
                            // std::cout<<"LFU"<<std::endl;
                            int min = 0;
                            for (int i = 1; i < lru.size(); i++)
                            {
                              if (lru[min].second > lru[i].second)
                                min = i;
                            }
                            Cache.erase(lru[min].first);
                            for (int i = min; i < lru.size() - 1; i++)
                            {
                              lru[i] = lru[i + 1];
                            }
                          }
                          // lru[last]=//new entry
                          // Cache.erase(key);
                          // for(int i=0;i<lru.size();i++)   cout<<lru[i].first<<" "<<lru[i].second<<" ";
                          // cout<<"\n";
                          lru.pop_back();
                          pthread_rwlock_unlock(&lock_cache);
                        }
                        // Cache[key]=key;
                        KeyValueEntry k1;
                        //from storage
                        k1.key = key;
                        k1.value = value;
                        // cout<<k1.value;
                        pthread_rwlock_wrlock(&lock_cache);
                        Cache.insert(make_pair(key, k1));
                        lru.push_back(make_pair(key, 1));
                        pthread_rwlock_unlock(&lock_cache);
                      }
                      else
                      {
                        // cout <<  key<<" Element Present in cache\n"; //update lru and lfu accordingly
                        int toupdate = 0;
                        Cache[key].value = value; //UPDATE IN CACHE lock on that value thing
                        put_key_val(key, value);  //update in PStorage lock on that file ONLY

                        pthread_rwlock_wrlock(&lock_cache);
                        for (int i = 0; i < lru.size(); i++)
                        {
                          if (string(lru[i].first) == key)
                          {
                            toupdate = i;
                            lru[i].second += 1; //see frequency later
                            break;
                          }
                        }
                        pair<string, int> temp = lru[toupdate];
                        for (int i = toupdate; i < lru.size() - 1; i++)
                        {
                          lru[i] = lru[i + 1];
                        }
                        // lru[0]=temp;
                        lru[lru.size() - 1] = temp;
                        pthread_rwlock_unlock(&lock_cache);

                        // cout<<"updated lru";
                      }

                      server_stat_code = "200";
                      server_reply = "(Key-\"" + key + "\", Value-\"" + value + "\") Stored Successfully";
                    }
                  }
                  else if (request_.type() == "DEL")
                  {

                    client_log_string += "Client-" + request_.client_id() + " : Request[\"" + request_.type();
                    client_log_string += "\", Parameters-(Key:\"" + key + "\")]" + "\n";

                    pthread_rwlock_wrlock(&lock_cache);
                    Cache.erase(key);
                    pthread_rwlock_unlock(&lock_cache);
                    string del_val = del_key(key);

                    int toupdate = 0;
                    int flag = 0;
                    pthread_rwlock_wrlock(&lock_cache);
                    for (int i = 0; i < lru.size(); i++)
                    {
                      if (string(lru[i].first) == key)
                      {
                        toupdate = i;
                        flag = 1;
                        break;
                      }
                    }
                    for (int i = toupdate; flag == 1 && i < lru.size() - 1; i++)
                    {
                      lru[i] = lru[i + 1];
                    }
                    if (flag == 1)
                      lru.pop_back();
                    pthread_rwlock_unlock(&lock_cache);

                    if (del_val != "NULL")
                    {
                      server_stat_code = "200";
                      server_reply += "(Key-\"" + key + "\", Value-\"" + del_val + "\") Deleted Successfully";
                    }
                    else
                    {
                      server_stat_code = "400";
                      server_reply = "Error : (Key-\"" + key + "\") Not Exists";
                    }
                  }
                }
                else
                {
                  // cout << "Now i will look at fingertable" << endl;

                  find_next_hop(key_id, next_hop_id, next_hop_port);
                  string target_str = default_ip + next_hop_port;
                  KeyValueServiceClient client(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
                  cout<<"Forwarded to "<<next_hop_id<<" "<<next_hop_port<<endl;
                  server_reply = client.GENERAL_KV(key, value, request_.type(), request_.client_id(), next_hop_port, request_.path() + to_string(id) + "->");
                  // reply_.set_message(server_reply);
                }
              }

              if (server_stat_code == "")
              {
                client_log_string += "Thread-" + to_string(server_id) + " : Response[ Forwarded request to server at port " + next_hop_port + "]\n";
                client_log_string += "Cache Keys - ";
                pthread_rwlock_rdlock(&lock_cache);
                for (auto ele : Cache)
                {
                  client_log_string += ele.first + " ";
                }
                pthread_rwlock_unlock(&lock_cache);
              }
              else
              {
                server_reply += " - [ Reply from server (" + to_string(id) + ", port=" + port_no + ") ]";
                client_log_string += "Thread-" + to_string(server_id) + " : Response[ (Status Code : " + server_stat_code + ") " + server_reply + "]\n";
                client_log_string += "Path-" + request_.path() + to_string(id) + "\n";
                client_log_string += "Cache Keys - ";
                pthread_rwlock_rdlock(&lock_cache);
                for (auto ele : Cache)
                {
                  client_log_string += ele.first + " ";
                }
                pthread_rwlock_unlock(&lock_cache);
              }
            }
          }

          pthread_rwlock_wrlock(&lock_log_file);
          log_file << client_log_string << endl
                   << endl;
          pthread_rwlock_unlock(&lock_log_file);

          reply_.set_message(server_reply);
          reply_.set_status_code(server_stat_code);
        }

        // And we are done! Let the gRPC runtime know we've finished, using the
        // memory address of this instance as the uniquely identifying tag for
        // the event.

        status_ = FINISH;
        responder_.Finish(reply_, Status::OK, this);
      }
      else
      {
        // std::cout<<"Finish"<<std::endl;
        GPR_ASSERT(status_ == FINISH);
        // Once in the FINISH state, deallocate ourselves (CallData).
        delete this;
      }
    }

  private:
    int REQUEST_TYPE;

    // The means of communication with the gRPC runtime for an asynchronous
    // server.
    KeyValueService::AsyncService *service_;
    // The producer-consumer queue where for asynchronous server notifications.
    ServerCompletionQueue *calldata_cq_;
    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    ServerContext ctx_;

    // What we get from the client.

    Join_Request join_request_;

    KV_Request request_;
    // PUT_KV_Request put_request_;
    // DEL_KV_Request del_request_;
    // What we send back to the client.
    KV_Reply reply_;

    // The means to get back to the client.
    ServerAsyncResponseWriter<KV_Reply> responder_;

    // Let's implement a tiny state machine with the following states.
    enum CallStatus
    {
      CREATE,
      PROCESS,
      FINISH
    };
    CallStatus status_; // The current serving state.
  };

  // This can be run in multiple threads if needed.
  void HandleRpcs(int i)
  {
    // Spawn a new CallData instance to serve new clients.
    new CallData(&service_, list_of_cqs_[i].get(), CLIENT_TYPE);
    new CallData(&service_, list_of_cqs_[i].get(), JOIN_TYPE);
    // new CallData(&service_, list_of_cqs_.get(), GET_TYPE);
    // new CallData(&service_, list_of_cqs_.get(), PUT_TYPE);
    // new CallData(&service_, list_of_cqs_.get(), DEL_TYPE);
    void *tag; // uniquely identifies a request.
    bool ok;
    while (true)
    {

      // std::cout<<"Thread Number - "<<i<<std::endl;
      // Block waiting to read the next event from the completion queue. The
      // event is uniquely identified by its tag, which in this case is the
      // memory address of a CallData instance.
      // The return value of Next should always be checked. This return value
      // tells us whether there is any kind of event or list_of_cqs_ is shutting down.
      GPR_ASSERT(list_of_cqs_[i]->Next(&tag, &ok));
      GPR_ASSERT(ok);
      static_cast<CallData *>(tag)->Proceed(i);
    }
  }

  std::vector<std::unique_ptr<ServerCompletionQueue>> list_of_cqs_;
  int num_threads;
  KeyValueService::AsyncService service_;
  std::unique_ptr<Server> server_;
};

int main(int argc, char **argv)
{

  if (argc > 1 && argc <= 2)
  {
    port_no = argv[1];

    id = hash_string(port_no);

    successor_port = port_no;
    successor_id = to_string(id);
    predecessor_port = port_no;
    predecessor_id = to_string(id);
    list_of_servers = to_string(id) + "-" + port_no;
    cout << "Server (" << id << ", " << port_no << ")" << endl;
    // cout << "First server started" << endl;
    // cout << "Successor - " << successor_port << " " << successor_id << endl;
    // cout << "Predecessor - " << predecessor_port << " " << predecessor_id << endl;
    // cout << "List - " << list_of_servers << endl;
    vector_list = separated_id_port(list_of_servers);
    // update_succ_pred();
    initialize_ft();
    print_fingertable();
  }
  else if (argc > 2)
  {
    port_no = argv[1];

    id = hash_string(port_no);
    string target_port = argv[2];

    //Server to Server send logic
    string target_str = default_ip + target_port;
    KeyValueServiceClient server_as_client = grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()); // Creating channel with target port
    KV_Reply server_bhayia = server_as_client.Server_Join(port_no, to_string(id), target_port, "NewServer", "");  // Sending data to target port

    predecessor_port = server_bhayia.predecessor_port();
    predecessor_id = server_bhayia.predecessor_id();
    successor_port = server_bhayia.successor_port();
    successor_id = server_bhayia.successor_id();
    list_of_servers = server_bhayia.list();

    vector_list = separated_id_port(list_of_servers);
    update_succ_pred();

    cout << "Server (" << id << ", " << port_no << ")" << endl;
    initialize_ft();
    print_fingertable();

    //send request to sucessor to give back his keys
    KeyValueServiceClient key_transfer_client = grpc::CreateChannel(default_ip + successor_port, grpc::InsecureChannelCredentials()); // Creating channel with target port
    KV_Reply key_transfer = key_transfer_client.Server_Join(port_no, to_string(id), "", "KeyTransfer", "");              // Sending data to target port

    // cout << server_bhayia.message() << endl;
    // cout << "Successor - " << successor_port << " " << successor_id << endl;
    // cout << "Predecessor - " << predecessor_port << " " << predecessor_id << endl;
    // cout << "List - " << list_of_servers << endl;
  }

  // int new_id = hash_string(port_no);
  // cout<<id<<" "<<new_id<<endl;
  pthread_rwlock_init(&lock_cache, NULL);
  pthread_rwlock_init(&lock_log_file, NULL);
  for (int i = 0; i < 128; i++)
    pthread_rwlock_init(&lockFile[i], NULL);
  ServerImpl server;
  server.Run();

  return 0;
}

int put_key_val(string key, string value)
{
  //cout<<key+" "+value<<endl;
  char first_char = key[0];
  int ascii = first_char;
  string ascii_str = to_string(ascii);

  //First check whether key already exists
  bool key_exists = 0;
  fstream file("../Output/KV_files/" + ascii_str + "_" + port_no + ".txt", ios::in | ios::out);

  //Total length of each entry is 256 (Key)+256(Value)+2 =514 bytes
  //2 extra bytes (one for valid status flag and one line break for readability)
  char buffer[514];
  pthread_rwlock_rdlock(&lockFile[ascii]);
  while (file.read(buffer, 514))
  {
    pthread_rwlock_unlock(&lockFile[ascii]);
    string extract_key = buffer;
    //cout<<"Extracted key is "+key_extract+" with length = "<<key_extract.length()<<endl;
    //if (extract_key==key) cout<<"PUT Key match found for "+key<<endl;

    //buffer[512] stores status flag of entry
    if (buffer[512] == '1' && extract_key == key)
    {
      key_exists = 1;
      //Reset Value to Zero before replacememnt
      memset(buffer + 256, 0, 256);
      //Store new value
      for (int i = 0; i < value.length(); i++)
      {
        buffer[i + 256] = value[i];
      }

      //Move back pointer to beginning of entry/row before updating
      pthread_rwlock_wrlock(&lockFile[ascii]);
      file.seekp(-514, ios::cur);
      file.write(buffer, 514);

      // log_file<<"Key "+key+ " already exists! It will be updated with value "+value<<endl;
      //Stop reading further lines (Break out of while loop)
      break;
    }
    pthread_rwlock_rdlock(&lockFile[ascii]);
  }
  pthread_rwlock_unlock(&lockFile[ascii]);
  file.close();
  //If key does not exist already, we look for deleted entry to replace else append new entry to the file
  if (!key_exists)
  {
    //cout<<"Key doesnt exist. Trying to insert!"<<endl;
    //This flag is set to 1 when a dead/deleted entry is found. We repplace the deleted entry with new entry
    //This avoids using an extra row at the end to append the new entry.
    bool append_flag = 0;
    //Open file in append mode to write at end of file
    fstream file("../Output/KV_files/" + ascii_str + "_" + port_no + ".txt", ios::in | ios::out);
    char K[256];
    memset(K, 0, 256);
    //Copy key into array K of fixed length
    for (int i = 0; i < key.length(); i++)
    {
      K[i] = key[i];
    }

    char V[256];
    memset(V, 0, 256);
    //Copy  value into array V of fixed length
    for (int i = 0; i < value.length(); i++)
    {
      V[i] = value[i];
    }

    //Reading each line
    //Set pointer to start
    //file.seekp(file.beg);
    char buffer[514];
    while (file.read(buffer, 514))

    {
      //Try looking for a DELETED entry and replace that entire line with the new entry
      if (buffer[512] == '0') // Means deleted and useless entry
      {
        //Move back pointer to beginning of entry/row before updating
        file.seekp(-514, ios::cur);
        //Writing Key/Value and 2 extra bytes
        file.write(K, 256);
        file.write(V, 256);
        //Writing one to indicate valid entry
        file.write("1", 1);
        //Adding a line break for readability
        file.write("\n", 1);
        append_flag = 1;

        //cout<<"DE FOUND!!"<<endl;
        // log_file<<"Successfully inserted key "+key+" with value "+value<<endl;
        //Stop reading further lines and exit while loop
        break;
      }
    }

    file.close();
    //If couldn't find a deleted row, then we ultimately append at the end only
    if (!append_flag)

    {
      ofstream file("../Output/KV_files/" + ascii_str + "_" + port_no + ".txt", ios::app);
      //Writing Key/Value and 2 extra bytes
      file.write(K, 256);
      file.write(V, 256);
      //Writing one to indicate valid entry
      file.write("1", 1);
      //Adding a line break for readability
      file.write("\n", 1);
      //cout<<"DE NOT FOUND!!"<<endl;
      // log_file<<"Successfully inserted key "+key+" with value "+value<<endl;
      file.close();
    }
  }
  return 0;
}

//GET function
string get_key(string key)
{

  char first_char = key[0];
  int ascii = first_char;
  string ascii_str = to_string(ascii);
  ifstream file("../Output/KV_files/" + ascii_str + "_" + port_no + ".txt");
  string extract_val;
  if (file.is_open())
  {
    int key_found = 0;
    char buffer[514];
    //
    // auto file_read_var=file.read(buffer,514);
    pthread_rwlock_rdlock(&lockFile[ascii]);
    while (file.read(buffer, 514))
    {
      pthread_rwlock_unlock(&lockFile[ascii]);
      string extract_key = buffer;
      if (buffer[512] == '1' && extract_key == key)
      {
        key_found = 1;
        char V[256];
        memset(V, 0, 256);
        for (int i = 0; i < 256; i++)
          V[i] = buffer[i + 256];
        //Print extracted
        extract_val = V;
        // log_file<<"Extracted value for key "+key+" is "+extract_val<<endl;;
        //Stop reading further lines (Exit while loop)
        break;
      }
      //
      // file_read_var=file.read(buffer,514);
      pthread_rwlock_rdlock(&lockFile[ascii]);
    }
    if (key_found != 1)
      pthread_rwlock_unlock(&lockFile[ascii]);
    //
    if (key_found == 0)
    {
      extract_val = "NULL";
    }
    file.close();
  }
  else
  {
    extract_val = "NULL";
  }
  return extract_val;
}

//Delete Function
string del_key(string key)
{

  //cout<<"Key to be deleted is "+key<<endl;;
  char first_char = key[0];
  int ascii = first_char;
  string ascii_str = to_string(ascii);
  fstream file("../Output/KV_files/" + ascii_str + "_" + port_no + ".txt", ios::in | ios::out);
  char buffer[514];
  bool del_flag = 0;
  string extract_val;

  pthread_rwlock_rdlock(&lockFile[ascii]);
  while (file.read(buffer, 514))
  {
    pthread_rwlock_unlock(&lockFile[ascii]);
    string extract_key = buffer;
    //Key to be deleted is FOUND!
    if (buffer[512] == '1' && extract_key == key)
    {
      //     string extract_key=buffer;
      // if (buffer[512]=='1' && extract_key==key)
      // {
      // key_found=1;
      char V[256];
      memset(V, 0, 256);
      for (int i = 0; i < 256; i++)
        V[i] = buffer[i + 256];

      //Print extracted
      extract_val = V;
      // log_file<<"Extracted value for key "+key+" is "+extract_val<<endl;;

      //Stop reading further lines (Exit while loop)
      // break;
      // }
      //cout<<"Complete match found!"<<endl;
      pthread_rwlock_wrlock(&lockFile[ascii]);
      file.seekp(-2, ios::cur);
      //Set flag=0 for DELETED Keys
      file.write("0\n", 2);
      //Set delete flag to 1
      del_flag = 1;
      // log_file<<"Successfully deleted key "+key<<endl;
      //Stop reading further lines (Exit while loop)
      break;
    }
    pthread_rwlock_rdlock(&lockFile[ascii]);
  }
  pthread_rwlock_unlock(&lockFile[ascii]);
  if (del_flag == 0)
  {
    extract_val = "NULL";
  }
  file.close();
  // else{extract_val="NULL";}
  return extract_val;

  //Add code if DEL key not found
  // if (!del_flag) log_file<<"Couldn't locate key "+key+" to be deleted!"<<endl;
  // file.close();

  // return del_flag;
}
