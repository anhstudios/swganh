
namespace anh
{
	namespace clientfile_manager
	{
		namespace iff
		{
			namespace interpreters
			{
				template <class T> std::shared_ptr<T> oiff_interpreter::getAttribute(std::string& key)
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

				template <class U> U oiff_interpreter::getAttribute(std::string& key, bool giveWithoutSharedPtr)
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
};