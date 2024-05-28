#include "SQLiteTabInfo.h"
using namespace cylDB;
QString SQLiteTabInfo::getName( ) const {
	return { };
}
bool SQLiteTabInfo::reName( const QString &new_name ) {
	return false;
}
bool SQLiteTabInfo::copyTo( const I_TabInfo *other_tab ) const {
	return false;
}
bool SQLiteTabInfo::removeTab( ) {
	return false;
}
bool SQLiteTabInfo::isValid( ) const {
	return false;
}
