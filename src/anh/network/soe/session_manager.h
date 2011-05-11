	typedef std::map<boost::asio::ip::udp::endpoint, std::shared_ptr<Session>>				SessionMap;
	typedef std::map<boost::asio::ip::udp::endpoint, std::shared_ptr<Session>>::iterator	SessionMapIterator;

	SessionMap	sessions_;