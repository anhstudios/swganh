
namespace ClientFileManager
{
	namespace IFF
	{
		namespace Interpreters
		{
			template <class T> std::shared_ptr<T> OIFFInterpreter::getAttribute(std::string& key)
			{
				try
				{
					std::map<std::string, boost::any>::const_iterator it = attributes_.find(key);
					if(it != attributes_.cend())
					{
						return boost::any_cast<std::shared_ptr<T>>(it->second);
					}
				}
				catch(const boost::bad_any_cast&) 
				{
					//A warning message would probably be nice here
				}
				return nullptr;
			}

			template <class U> U OIFFInterpreter::getAttribute(std::string& key, bool giveWithoutSharedPtr)
			{
				try
				{
					std::map<std::string, boost::any>::const_iterator it = attributes_.find(key);
					if(it != attributes_.cend())
					{
						return boost::any_cast<U>(it->second);
					}
				}
				catch(const boost::bad_any_cast&) 
				{
					//A warning message would probably be nice here
				}
				return nullptr;
			}
		};
	};
};