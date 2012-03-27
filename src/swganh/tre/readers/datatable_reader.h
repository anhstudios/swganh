// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_TRE_READERS_DATATABLE_READER_H_
#define SWGANH_TRE_READERS_DATATABLE_READER_H_

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <boost/any.hpp>

namespace swganh {
namespace tre {
namespace readers {

    namespace detail {
        /**
         * Attempts to cast the cell's value to the specified type T.
         *
         * \return The cell's value cast as the given type T.
         */
        template<typename T>
        T GetValue(const boost::any& value)
        {
            return *boost::any_cast<const T*>(value);
        }

        /**
         * An overload of GetValue that can turn char* into string.
         *
         * \return The cell's value as a string.
         */
        template<>
        inline std::string GetValue<std::string>(const boost::any& value)
        {
            return std::string(boost::any_cast<const char*>(value));
        }
    }

    /**
     * Represents a single cell value in a datatable row.
     */
    class DatatableCell
    {
    public:
        /**
         * \return String representation of cell's value.
         */
        std::string ToString() const;

        /**
         * Attempts to cast the cell's value to the specified type T.
         *
         * \return The cell's value cast as the given type T.
         */
        template<typename T>
        T GetValue() const
        {
            return detail::GetValue<T>(value_);
        }

        /**
         * Set's the value of this cell.
         *
         * \param The new value of the cell.
         */
        template<typename T>
        void SetValue(T value)
        {
            value_ = value;
        }

    private:
        boost::any value_;
    };

    /**
     * A utility class for parsing files in the datatable format.
     */
    class DatatableReader
    {
    public:
        /**
         * Explicit constructor that takes ownership of a resource.
         */
        explicit DatatableReader(std::vector<char>&& input);

        /**
         * \return The number of rows in this datatable.
         */
        uint32_t CountRows() const;

        /**
         * \return A list of all column names in the order they appear.
         */
        const std::vector<std::string>& GetColumnNames() const;

        /**
         * Increments the datatable reader to the next row. Initially starts
         * at position -1.
         *
         * \code.cpp
         *     DatatableReader reader(...);
         *     while (reader->Next())
         *     {
         *         auto row = reader->GetRow();
         *
         *         // ... work with row data
         *     }
         *
         * \return True if able to increment to the next row, false if not.
         */
        bool Next();

        /**
         * Returns the row at the current position. Fields in the row are
         * accessed via their string label.
         *
         * \return The current row.
         */
        std::map<std::string, DatatableCell*> GetRow();

    private:
        void ValidateFile(const std::vector<char>& input) const;

        void ParseColumnNames();
        void ParseColumnTypes();
        void ParseRows();
        uint32_t ParseColumn(uint8_t type, uint32_t offset, std::vector<DatatableCell>& row);

        struct ColumnHeader {
            char name[4];
            uint32_t size;
            uint32_t count;
        };

        struct TypeHeader {
            char name[4];
            uint32_t size;
        };

        struct RowHeader {
            char name[4];
            uint32_t size;
            uint32_t count;
        };

        int32_t current_row_;

        std::vector<char> input_;

        const ColumnHeader* column_header_;
        const char* column_offset_;
        std::vector<std::string> column_names_;

        const TypeHeader* type_header_;
        const char* type_offset_;
        std::vector<std::string> column_types_;

        const RowHeader* row_header_;
        const char* row_offset_;

        std::vector<std::vector<DatatableCell>> row_data_;
    };

}}}  // namespace swganh::tre::readers

#endif  // SWGANH_TRE_READERS_DATATABLE_READER_H_
