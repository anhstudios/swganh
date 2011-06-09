
namespace anh
{
	namespace clientfile_manager
	{
		namespace interpreters
		{
			template <class T> std::shared_ptr<T> oiff_interpreter::attribute(std::string& key)
			{
				std::map<std::string, boost::any>::const_iterator it = attributes_.find(key);
				if(it != attributes_.cend())
				{
					return boost::any_cast<std::shared_ptr<T>>(it->second);
				}
				return nullptr;
			}
		};
	};
};