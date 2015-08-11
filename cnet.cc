#define BUFSIZE 4096
#include <node.h>
#include <v8.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


using namespace v8;

void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

void Connect(const FunctionCallbackInfo<Value>& args){
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    if (args.Length() != 2) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments, Usage ([host],[port])")));
      return;
    }
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(*String::Utf8Value(args[0]->ToString()));
    servaddr.sin_port=htons(args[1]->NumberValue());
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    Local<Number> num = Number::New(isolate, sockfd);
    args.GetReturnValue().Set(num);
}

void Write(const FunctionCallbackInfo<Value>& args){
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    int sock;
    	Local<String> data;
    	int start = 0, remaining, batch;
    	int nBytes = 0, nBatch;

    	if (args.Length() < 1) {
    		 isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,"socket and data are not specified")));
    		return ;
    	} else if (args.Length() < 2) {
    		 isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,"data is not specified")));
    		return ;
    	}
    	if (!args[0]->IsNumber()) {
    		 isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,"socket is not a number")));
    		return ;
    	}
    	if (!args[1]->IsString()) {
    		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,"data is not a string")));
    		return;
    	}

    	sock = args[0]->NumberValue();
    	data = args[1]->ToString();
    	remaining = data->Length();

    	// In case the data is larger than our buffer, we write them in batches
    	while (remaining > 0) {
            uint8_t* buf = new uint8_t[BUFSIZE];
    		batch = remaining <= BUFSIZE - 1 ? remaining : BUFSIZE - 1;

    		data->WriteOneByte(buf, start, batch);

    		nBatch = send(sock, buf, batch, 0);
    		if (nBatch == -1) {
    		    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,"send() failed")));
    			return ;
    		} else if (nBatch < batch) {
    			nBytes += nBatch;
    			break;
    		}
    		start += batch;
    		remaining -= batch;
    		nBytes += batch;
    	}
  Local<Number> num = Number::New(isolate, nBytes);
     args.GetReturnValue().Set(num);

}

void init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "hello", Method);
  NODE_SET_METHOD(exports,"connect",Connect);
  NODE_SET_METHOD(exports,"write",Write);
}

NODE_MODULE(cnet, init)