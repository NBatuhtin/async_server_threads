#include <cstdlib>               
#include <cstring>               
#include <iostream>              
#include <boost/asio.hpp>        
#include <thread>                
#include <vector>                

using boost::asio::ip::tcp;      
enum { max_length = 1024 };      

void client_thread(const char* host, const char* port, int num_cycles) {
    try {                         
        boost::asio::io_context io_context;  
        tcp::socket s(io_context);           
        tcp::resolver resolver(io_context);  
        boost::asio::connect(s, resolver.resolve(host, port));  

        for (int i = 0; i < num_cycles; ++i) {  
            std::cout << "Enter message: ";
            char request[max_length];            // Создание массива для запроса
            std::cin.getline(request, max_length);
            size_t request_length = std::strlen(request);
            boost::asio::write(s, boost::asio::buffer(request, request_length));  // Отправка сообщения

            char reply[max_length];  // Создание массива для ответа
            size_t reply_length = boost::asio::read(s, boost::asio::buffer(reply, request_length));
            std::cout << "Reply is: ";
            std::cout.write(reply, reply_length);  // Вывод ответа на экран
            std::cout << std::endl;
        }
    }
    catch (std::exception& e) {  
        std::cerr << "Exception: " << e.what() << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {  
        std::cerr << "Usage: blocking_tcp_echo_client <host> <port> <num_threads> <num_cycles>\n"; 
        return 1;  
    }

    const char* host = argv[1];                
    const char* port = argv[2];                
    int num_threads = std::atoi(argv[3]);  
    int num_cycles = std::atoi(argv[4]);  

    std::vector<std::thread> threads;  // Создание вектора для хранения потоков
    for (int i = 0; i < num_threads; ++i) {  
        threads.emplace_back(client_thread, host, port, num_cycles);  // Создание и добавление потоков
        std::cout << std::endl;
    }

    for (auto& thread : threads) {  // Ожидание завершения всех потоков
        thread.join();
    }

    return 0;  
}