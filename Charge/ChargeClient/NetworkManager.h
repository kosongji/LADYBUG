#pragma once
#include <vector>
#include <queue>



namespace CS
{
	class NetworkManager
	{
	public:
		NetworkManager();
		~NetworkManager();

		bool		Initialize();
		void		Finalize();

		void		CreateThread();

		void		AddUserCount() { InterlockedIncrement(&m_UserCount); }
		SOCKET		GetListenSocket() const { return m_ClientSocket; }

		static void	WorkerThread(CS::NetworkManager* p);

		void		OnProcessPakcet(char* ); 
		bool		OnSend();
		bool		OnRecv();
	private:
		std::vector<std::thread>	m_Threads;

		SOCKET						m_ClientSocket;
		LONG						m_UserCount;
		
		std::queue<packet*>			m_MessageQueue;
		//BUG_OBJ_POS					m_Player[2];
	};
}