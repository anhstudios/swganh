// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef VISITORS_OBJECT_VISITOR_H
#define VISITORS_OBJECT_VISITOR_H

#include "../visitor_interface.h"

#include <map>
#include <unordered_map>
#include <set>
#include <functional>
#include <cstdint>
#include <memory>
#include <boost/any.hpp>

namespace anh
{
namespace resource
{
	class ResourceManagerInterface;
}
}

namespace swganh
{
namespace tre
{

	class ObjectVisitor;
	//End Forward Declarations

	//Begin Typedefs
	typedef std::function<void(ObjectVisitor*, std::string&, anh::ByteBuffer&)> AttributeFunctor;
	typedef std::map<std::string, AttributeFunctor> AttributeHandlerIndex;
	typedef std::map<std::string, AttributeFunctor>::const_iterator AttributeHandlerIndexIterator;
	typedef std::map<std::string, std::shared_ptr<boost::any>> AttributeMap;
	//End Typedefs

	/**
		@brief An IFFVisitor for object iff files.
	*/
	class ObjectVisitor : public VisitorInterface
	{
	public:
		/**
			@brief Constructor for an object iff visitor
		*/
		ObjectVisitor();
				
		/**
			@brief returns the VisitorType associated with this visitor
		*/
		virtual VisitorType getType() { return OIFF_VISITOR; }
				
		/**
			@brief interprets a IFF::FileNode associated with this visitor.
			This should only be called by the IFFFile code.
		*/
		virtual void visit_data(std::shared_ptr<file_node> node);

		/**
			@brief interprets a IFF::FolderNode associated with this visitor.
			This should only be called by the IFFFile code.
		*/
		virtual void visit_folder(std::shared_ptr<folder_node> node);

		/**
			@brief An internal ClientString structure. This could later be moved outside this class.
			It is merely here now for completeness.
		*/
		static class ClientString
		{
		public:
			std::string file;
			std::string entry;
		};

		/**
			@brief returns an attribute loaded from the object iff associated with this interpreter.

			@param key the key to lookup

			@return a boost::any casted shared_ptr<T> that was requested
		*/
		template <class T> T attribute(const std::string& key);
				
		std::uint32_t attribute_uint32(std::string& key);
		float attribute_float(std::string& key);
		bool attribute_bool(std::string& key);


		/**
			@brief A function which causes this OIFFInterpreter to load parent files it needs and then copy
			their values downstream

			@param f the filemanager instance for retriving the necessary files.
		*/
		void load_aggregate_data(anh::resource::ResourceManagerInterface* f);

		/**
			@brief Causes this OIFFInterpreter to load any referenced files it has. It will use the filemanager
			to accomplish this.

			@param f the filemanager instance for retrieving the necessary files.
		*/
		void load_referenced_files(anh::resource::ResourceManagerInterface* f);

		/**
			@brief a simple output function for debugging values.
		*/
		void debug();

	private:
		//Internal Index used to link the handlers with the attributes
		static AttributeHandlerIndex attributeHandler_;

		//Internal Datatype handling functions used to parse the object files
		static void _handleUnhandled(ObjectVisitor* dst, std::string& name, anh::ByteBuffer& buf);
		static void _handleClientString(ObjectVisitor* dst, std::string& name, anh::ByteBuffer& buf);
		static void _handleString(ObjectVisitor* dst, std::string& name, anh::ByteBuffer& buf);
		static void _handleInt(ObjectVisitor* dst, std::string& name, anh::ByteBuffer& buf);
		static void _handleFloat(ObjectVisitor* dst, std::string& name, anh::ByteBuffer& buf);
		static void _handleBool(ObjectVisitor* dst, std::string& name, anh::ByteBuffer& buf);

		//Internal Node Handling functions
		void _handleXXXX(anh::ByteBuffer& buf);
		void _handleDERVXXXX(anh::ByteBuffer& buf);

		//Attributes this object iff might have
		AttributeMap attributes_;
				
		//Parent files this object iff might have
		std::set<std::string> parentFiles;

		//Stored for determing if we've already loaded our aggregate information.
		bool has_aggregate_;
		bool loaded_reference_;
	};
}
}

#include "object_visitor-intl.h"

#endif