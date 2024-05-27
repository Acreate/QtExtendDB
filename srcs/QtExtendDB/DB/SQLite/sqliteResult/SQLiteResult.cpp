#include "SQLiteResult.h"
using namespace cylDB;
SQLiteResult::SQLiteResult( ) {
	tabSPtr = std::make_shared< UN_Tab_Map >( );
	readTabIndex = currentCount = currentTabIndex = 0;

	currentSharedPtr = std::make_shared< Vector_VarSPtr >( );
	tabSPtr->emplace( currentTabIndex, currentSharedPtr );
}
SQLiteResult::~SQLiteResult( ) {

}

const Vector_VarSPtr_Shared SQLiteResult::getRows( const size_t index ) const {
	auto iterator = tabSPtr->begin( );
	auto end = tabSPtr->end( );
	for( ; iterator != end; ++end )
		if( iterator->first == index )
			return iterator->second;
	return nullptr;
}
const Var_Shared SQLiteResult::get( const size_t row, const size_t col ) const {
	auto iterator = tabSPtr->begin( );
	auto end = tabSPtr->end( );
	for( ; iterator != end; ++end )
		if( iterator->first == col ) {
			auto element = iterator->second.get( );
			if( element->size( ) > col )
				return element->at( col );
			return nullptr;
		}
	return nullptr;
}
