#include "DBTools.h"

#include "dbInterface/I_DB.h"
#include "SQLite/SQliteDB.h"

cylDB::DB_Shared cylDB::DBTools::linkDB( const QString &link, Super_DB_Type db_type ) {
	switch( db_type ) {
		case SQLite : {
			return std::make_shared< SQliteDB >( link );
			break;
		}
	}
	return nullptr;
}
