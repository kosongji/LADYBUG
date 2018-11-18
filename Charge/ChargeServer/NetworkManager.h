#pragma once
#include <vector>




	class NetworkManager
	{
	public:
		NetworkManager();
		~NetworkManager();

		bool			Initialize();
		void			Finalize();

		void			AcceptLoop();
		void			CreateThread(Session* sesion);

		inline void		AddUserCount() { InterlockedIncrement(&m_UserCount); }
		inline SOCKET	GetListenSocket() const { return m_ClientSocket; }

		static void		WorkerThread(Session* sessoin, NetworkManager*);

		void			OnProcessPakcet(char*);
		bool			OnSend();
		bool			OnRecv();
	private:
		std::vector<std::thread>	m_Threads;
		SOCKET						m_ClientSocket;
		LONG						m_UserCount;

	};
