#pragma once
#include <vector>

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	bool		Initialize();
	void		Finalize();

	void		AcceptLoop();
	void		CreateThread( Session* sesion );

	inline void		AddUserCount()	  { InterlockedIncrement( &m_UserCount ); }
	inline SOCKET	GetListenSocket() const {
		return m_ClientSocket
			;
	}

	static void		WorkerThread(Session* sessoin);

private:
	std::vector<std::thread>	m_Threads;
	SOCKET						m_ClientSocket;
	LONG						m_UserCount;

	// 객체들
	// 플레이어[2]
	// 버그[100]
	// 아이템[2]
	




};

