#include "DBTools.h"
cylDB::DB_Shared cylDB::DBTools::linkDB( const QString &link, Super_DB_Type db_type ) {
	switch( db_type ) {
		case Local_File : {

			break;
		}
		case SQLite : {
			break;
		}
	}
	return nullptr;
}
