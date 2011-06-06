#include <exception>
#include <string>

namespace anh
{
	namespace clientfile_manager
	{
		class NotFoundException : public std::exception
		{
		public:
			NotFoundException(std::string message)
				: message_(message)
			{
			}
			
			virtual const char* what() const throw() { return message_.c_str();} 
		private:
			std::string message_;
		};
	};
};