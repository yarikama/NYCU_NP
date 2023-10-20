/*
1) 先判斷有沒有照格式輸入ip
2) 創建socket (ipv[4,6], [tcp, udp], 0)
3) 判斷有沒有成功建立socket 
4) 清空然後建立能夠放入port跟ip地址的sockaddr_in  
    1) 從host傳給網路想要的port （因為[Big/Little]endian的關係，從主機傳出去網路 host to net，跟從網路接受到者機 net to host）
    2) 指定ip的類型
    3) 將ip地址放入 (pton 指的是 presentation to number，可以想成string to number就好)
    4) 檢查轉換有無成功
5) 將socket跟剛剛填好的ip地址和port進行連線（開啟three-way handshake）
    1) 建立connet
    2) 看看連線有沒有成功
6) 讀取跟印出socket中的東西
    1) 先建立read buffer
    2) 從socket中read，並將讀到的字存入buffer，然後再看看讀到幾個字（使用while是因為，要確保讀到所有數據，有時候數據是一個個傳到的，不會一次傳完）
    3) 讀到的字數假如
        1) >0 : 讀到字，請繼續讀
        2) =0 : 讀完了，可以結束while迴圈了
        3) <0 : 讀取發生錯誤
    4) 從buffer將讀到的內容輸出
        1) 若是讀到東西，卻沒輸出（EOF）就產生錯誤訊息
*/

#include <stdio.h>
#include <stdlib.h>       // exit
#include <string.h>       // memset
#include <sys/types.h>    // struct sockaddr_in, struct sockaddr
#include <sys/socket.h>   // read, socket, connet
#include <netinet/in.h>   // htons, AF_INET, SOCK_STREAM
#include <arpa/inet.h>    // inet_pton

int main(int argc, char** argv){
    if(argc != 2){
        fprintf(stderr, "你好像沒打ipㄛ");
        exit(1);
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){
        fprintf(stderr, "呼叫Socket錯誤");
        exit(2);    
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_port = htons(13);
    servaddr.sin_family = AF_INET;
    int pton = inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    if(pton < 0){
        fprintf(stderr, "轉換和傳入地址失敗");
        exit(3);
    }

    int cnt = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(cnt < 0){
        fprintf(stderr, "連線失敗");
        exit(4);
    }

    char read_buf[1024];
    int wc;
    while((wc = read(sockfd, read_buf, 1024)) > 0){
        read_buf[wc] = 0;
        if(fputs(read_buf, stdout) == EOF) fprintf(stderr, "印出時產生錯誤");
    }
    if(wc < 0) fprintf(stderr, "讀取錯誤");
    exit(0);
}