// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef DATATABLE_VISITOR_H
#define DATATABLE_VISITOR_H

#include "../visitor_interface.h"

#include <map>
#include <string>
#include <list>
#include <vector>
#include <cstdint>
#include <boost/any.hpp>

namespace swganh
{
namespace tre
{
	class DatatableVisitor : public VisitorInterface
	{
	public:
		/**
			@brief returns the VisitorType associated with this Visitor
		*/
		virtual VisitorType getType() { return DATATABLE_VISITOR; }

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

		class DATA_ROW
		{
		public:

			template <typename T>
			T GetValue(int column_id) {
				return boost::any_cast<T>(columns.at(column_id));
			}

			std::vector<boost::any> columns;
		};

		const std::vector<std::string>& column_names() { return column_names_; }
		const std::vector<char>& column_types() { return column_types_; }
					
		std::list<DATA_ROW>::iterator begin_itr() { return rows_.begin(); }
		std::list<DATA_ROW>::iterator end_itr() { return rows_.end(); }

	private:
		void _handle0001COLS(anh::ByteBuffer& buf);
		void _handleTYPE(anh::ByteBuffer& buf);
		void _handleROWS(anh::ByteBuffer& buf);

		std::vector<char> column_types_;
		std::vector<std::string> column_names_;
		std::list<DATA_ROW> rows_;
	};
}
}

#endif