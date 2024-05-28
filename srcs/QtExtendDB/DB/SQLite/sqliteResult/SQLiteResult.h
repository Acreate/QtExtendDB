#ifndef SQLITERESULT_H_H_HEAD__FILE__
#define SQLITERESULT_H_H_HEAD__FILE__
#pragma once

#include "../../../nameSpace/cylDB.h"
#include "../../dbInterface/i_resultInfo/I_ResultInfo.h"
namespace cylDB {
	class QTEXTENDDB_EXPORT SQLiteResult : public I_ResultInfo {
		friend class SQliteDepository;
		friend class SQLiteTabInfo;
	public:
	private:
		UNTabMap_Shared tabSPtr;
		Vector_VarSPtr_Shared currentSharedPtr;
		size_t currentTabIndex;
		size_t readTabIndex;
		size_t currentCount;
	public:
		SQLiteResult( );
		~SQLiteResult( ) override;
		size_t getCount( ) const override {
			return currentCount;
		}
		size_t getColCount( ) const override {
			return tabSPtr->size( );
		}
		const Vector_VarSPtr_Shared getRows( const size_t index ) const override;
		const Var_Shared get( const size_t row, const size_t col ) const override;
		const Vector_VarSPtr_Shared getCurrentRows( ) const override {
			return currentSharedPtr;
		}
		void clearCurrentCol( ) override {
			currentSharedPtr->clear( );
		}
		size_t currentIndex( ) const override {
			return currentTabIndex;
		}
		void resetColIndex( ) override {
			readTabIndex = 0;
			currentSharedPtr = tabSPtr->at( readTabIndex );
		}
		void clear( ) override {
			tabSPtr->clear( );
			currentTabIndex = 0;
		}
		size_t currentCol( ) override {
			return readTabIndex;
		}
		bool nextCol( ) override {
			if( readTabIndex >= currentTabIndex )
				return false;
			++readTabIndex;
			currentSharedPtr = tabSPtr->at( readTabIndex );
			return true;
		}
	public:
		void newCol( ) {
			++currentTabIndex;
			currentSharedPtr = std::make_shared< Vector_VarSPtr >( );
			tabSPtr->emplace( currentTabIndex, currentSharedPtr );
		}
		void appendRwo( const QVariant &variant ) {
			currentSharedPtr->emplace_back( std::make_shared< QVariant >( variant ) );
			++currentCount;
		}
	};
}
#endif // SQLITERESULT_H_H_HEAD__FILE__
