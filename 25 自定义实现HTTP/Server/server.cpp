#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <iostream>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void* request_handler(void* arg);
void send_data(FILE* fp, char* ct, char* file_name);
const char* content_type(char* file);
void send_error(FILE* fp);
void error_handling(char* message);

int main(int argc, char *argv[]){
    pthread_t t_id;
    char buf[BUF_SIZE];
    socklen_t clnt_adr_size;
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    int on = 1;
    setsockopt( serv_sock , SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port = htons(9190);
	if( bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1 ) {
        std::cout << "bind socket failed." << std::endl;
        exit(1);
    }

	if( listen(serv_sock, 20) == -1 ){
        std::cout << "listen socket failed." << std::endl;
        exit(1);
    }

	while(1){
		clnt_adr_size = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_size);
        // 采用多线程处理客户端的HTTP请求
		pthread_create(&t_id, NULL, request_handler, &clnt_sock);
		pthread_detach(t_id);
	}

	close(serv_sock);
	return 0;
}

void* request_handler(void *arg){
    char ct[15];
    char method[10];
    char file_name[30];
    FILE* clnt_read;
    FILE* clnt_write;
	int clnt_sock=*((int*)arg);
	char req_line[SMALL_BUF];
	
	clnt_read=fdopen(clnt_sock, "r");
	clnt_write=fdopen(dup(clnt_sock), "w");
    // 获得HTTP的请求头。example: GET /index.html HTTP/1.1
	fgets(req_line, SMALL_BUF, clnt_read);	
    
    // 判断请求头中是否包含HTTP
	if( strstr(req_line, "HTTP/") == NULL ) {
		send_error(clnt_write);
		fclose(clnt_read);
		fclose(clnt_write);
		return NULL;
	 }
	
	strcpy( method, strtok(req_line, " /") );
	strcpy( file_name, strtok(NULL, " /") );
    // 依据文件名后缀获取文件的类型
	strcpy(ct, content_type(file_name));

    // 请求类型不满足时的处理办法
	if( strcmp(method, "GET") != 0 ){
		send_error(clnt_write);
		fclose(clnt_read);
		fclose(clnt_write);
		return NULL;
	 }

	fclose(clnt_read);
	send_data(clnt_write, ct, file_name); 
    return NULL;
}

void send_data(FILE* fp, char* ct, char* file_name){
	char protocol[]="HTTP/1.0 200 OK\r\n";
	char server[]="Server:Linux Web Server \r\n";
	char cnt_len[]="Content-length:2048\r\n";
	char cnt_type[SMALL_BUF];
	char buf[BUF_SIZE];
	FILE* send_file;
	
    // 消息头和消息消息体之间存在一个空行，故用2次\r\n\r\n。
	sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
	send_file = fopen(file_name, "r");
	if( send_file == NULL){
		send_error(fp);
		return;
	}

	// 发送状态行和消息头数据给客户端
	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);

	// 将消息体的数据发送给客户端
	while(fgets(buf, BUF_SIZE, send_file)!=NULL) {
		fputs(buf, fp);
		fflush(fp);
	}
	fflush(fp);

	fclose(fp);
}

// 函数功能: 根据文件后缀返回文件的类型
const char* content_type(char* file){
	char extension[SMALL_BUF];
	char file_name[SMALL_BUF];
	strcpy( file_name, file );
	strtok( file_name, "." );
	strcpy( extension, strtok( NULL, "." ) );
	
	if( !strcmp(extension, "html") || !strcmp(extension, "htm") ) 
		return "text/html";
	else
		return "text/plain";
}

// 函数功能: 处理错误时的提示信息
void send_error(FILE* fp){	
	char protocol[]="HTTP/1.0 400 Bad Request\r\n";
	char server[]="Server:Linux Web Server \r\n";
	char cnt_len[]="Content-length:2048\r\n";
	char cnt_type[]="Content-type:text/html\r\n\r\n";
	char content[]="<html><head><title>NETWORK</title></head>"
	       "<body><font size=+5><br> bad request"
		   "</font></body></html>";

	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);
    fputs(content,fp);
	fflush(fp);
}