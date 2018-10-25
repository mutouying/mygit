#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>

class process
{
	public:
		process():m_pid(-1){}
		
	public:
		pid_t m_pid;
		int m_pipefd[2];
};

template <typename T>
class processpool
{
	private:
		processpool(int listenfd, int process_number = 8);
	public:
		static processpool<T>* create(int listenfd, int process_number = 8)
		{
			if(!m_instance)
			{
				m_instance = new processpool<T>(listenfd, process_number);
			}
			return m_instance;
		}
		~processpool()
		{
			delete [] m_sub_process;
		}
		void run();
	private:
		void setup_sig_pipe();
		void run_parent();
		void run_child();
	private:
		static const int MAX_PROCESS_NUMBER = 16;
		static const int USER_PER_PROCESS = 65536;
		static const int MAX_EVENT_NUMBER = 10000;
		int m_process_number;
		int m_idx;
		int m_epollfd;
		int m_listenfd;
		int m_stop;
		process* m_sub_process;
		static processpool<T>* m_instance;
};

template< typename T >
processpool< T >* processpool<T>::m_instance = NULL;

//用于处理信号的管道，以实现统一事件源，后面称之为信号管道
static int sig_pipefd[2];

static int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

static void addfd(int epollfd, int fd)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLET;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	setnonblocking(fd);
}

//从epollfd标识的epoll内核事件表中删除fd上的所有注册事件
static void removefd(int epollfd, int fd)
{
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
	close(fd);	
}

static void sig_handler(int sig)
{
	int save_errno = errno;
	int msg = sig;
	send(sig_pipefd[1], (char*)&msg, 1, 0);
	errno = save_errno;
}

static void addsig(int sig, void(handler)(int), bool restart = true)
{
	struct sigaction sa;
	memset(&sa, '\0', sizeof(sa));
	sa.sa_handler = handler;
	if(restart)
	{
		sa.sa_flags |= SA_RESTART;
	}
	sigfillset(&sa.sa_mask);
	assert(sigaction(sig, &sa, NULL) != -1);
}

template<typename T>
processpool<T>::processpool(int listenfd, int process_number)
:m_listenfd(listenfd), m_process_number(process_number), m_idx(-1),m_stop(false)
{
	assert((process_number >0) && (process_number <= MAX_PROCESS_NUMBER));
	
	
}
































