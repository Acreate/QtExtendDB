#include "SQliteDepository.h"
#include "../sqliteResult/SQLiteResult.h"
#include <qfileinfo.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlResult>
#include <QSqlQuery>
#include <QMap>

using namespace cylDB;

inline std::shared_ptr< QSqlDatabase > make_QSqlDatabase( const QString &connectionName = "qt_sql_default_connection" ) {
	std::shared_ptr< QSqlDatabase > result;
	if( QSqlDatabase::contains( connectionName ) )
		result = std::shared_ptr< QSqlDatabase >( new QSqlDatabase( QSqlDatabase::database( connectionName ) )
			, []( QSqlDatabase *p ) {
				if( p->isOpen( ) )
					p->close( );
				delete p;
			} );
	else
		result = std::shared_ptr< QSqlDatabase >( new QSqlDatabase( QSqlDatabase::addDatabase( "QSQLITE", connectionName ) )
			, []( QSqlDatabase *p ) {
				if( p->isOpen( ) )
					p->close( );
				delete p;
			} );
	return result;
}

inline IResultInfo_Shared get_QSqlQuery_result( QSqlQuery &q_sql_result ) {
	bool next = q_sql_result.next( );
	if( next ) {
		SQLiteResult *sqLiteResult = new SQLiteResult;
		do {
			for( int index = 0; true; ++index ) {
				auto value = q_sql_result.value( index );
				if( !value.isValid( ) )
					break;
				sqLiteResult->appendRwo( value );
			}
			sqLiteResult->newCol( );
			next = q_sql_result.next( );
		} while( next );
		return std::shared_ptr< I_ResultInfo >( sqLiteResult );
	}
	return nullptr;
}
inline bool is_QSqlQuery_run( const QSqlDatabase *database, const QString &cmd ) {
	QSqlQuery query( *database );
	bool exec = query.exec( cmd );
	if( !exec )
		qDebug( ) << cmd << " : " << query.lastError( ).text( ).toStdString( ).c_str( );
	return exec;
}
inline IResultInfo_Shared get_QSqlQuery_run( QSqlDatabase *database, const QString &cmd ) {
	QSqlQuery query( *database );
	bool exec = query.exec( cmd );
	if( exec )
		return get_QSqlQuery_result( query );
	qDebug( ) << cmd << " : " << query.lastError( );
	return nullptr;
}
inline qsizetype append_map( QString &cmd, const QVariantMap &append_map ) {
	qsizetype result = 0;
	auto iterator = append_map.begin( );
	auto end = append_map.end( );
	do {
		cmd.append( " `" );
		cmd.append( iterator.key( ) );
		cmd.append( "` " );
		cmd.append( iterator.value( ).toString( ) );
		++iterator;
		++result;
		if( iterator == end )
			break;
		cmd.append( ", " );
	} while( true );

	return result;
}
inline qsizetype append_name( QString &cmd, const QStringList &append_list ) {
	qsizetype result = 0;
	auto iterator = append_list.begin( );
	auto end = append_list.end( );
	do {
		cmd.append( " `" );
		cmd.append( *iterator );
		cmd.append( "`" );
		++iterator;
		++result;
		if( iterator == end )
			break;
		cmd.append( " , " );
	} while( true );

	return result;
}
inline qsizetype append_value( QString &cmd, const QStringList &append_list ) {
	qsizetype result = 0;
	auto iterator = append_list.begin( );
	auto end = append_list.end( );
	do {
		cmd.append( *iterator );
		++iterator;
		++result;
		if( iterator == end )
			break;
		cmd.append( ", " );
	} while( true );

	return result;
}
cylDB::SQliteDepository::SQliteDepository( const QString &link_path, const QString &user, const QString &password ) {

	QFileInfo fileInfo( link_path );
	linkPath = fileInfo.absoluteFilePath( );
	this->user = user;
	this->password = password;


	if( fileInfo.exists( ) && fileInfo.isFile( ) ) {
		database = make_QSqlDatabase( linkPath );
		database->setDatabaseName( linkPath );
	} else
		database = nullptr;
}
cylDB::SQliteDepository::SQliteDepository( const QString &link_path ) {
	QFileInfo fileInfo( link_path );
	if( fileInfo.exists( ) && fileInfo.isFile( ) ) {
		linkPath = fileInfo.absoluteFilePath( );
		database = make_QSqlDatabase( linkPath );
		database->setDatabaseName( linkPath );
	}
}
cylDB::SQliteDepository::~SQliteDepository( ) {
}
cylDB::SQliteDepository::SQliteDepository( const std::shared_ptr< QSqlDatabase > &database, const QString &db_path ) {
	this->database = database;
	linkPath = db_path;
}
QString cylDB::SQliteDepository::getDBName( ) const {
	if( database )
		return linkPath;
	return "";
}
bool cylDB::SQliteDepository::hasTab( const QString &tab_name ) const {
	return database->tables( ).contains( tab_name );
}
bool cylDB::SQliteDepository::createTab( const QString &tab_name, const QVariantMap &tab_info, const QString &key, const QString &key_type ) const {
	if( !database || tab_info.isEmpty( ) || tab_name.isEmpty( ) )
		return false;
	if( key.isEmpty( ) )
		return createTab( tab_name, tab_info );

	QString cmd( R"(CREATE TABLE IF NOT EXISTS `%1` ( )" );
	cmd = cmd.arg( tab_name );

	cmd.append( '`' );
	cmd.append( key );
	cmd.append( "` " );
	cmd.append( key_type );
	cmd.append( "  PRIMARY KEY AUTOINCREMENT, " );
	append_map( cmd, tab_info );
	cmd.append( u8" ); " );
	return is_QSqlQuery_run( database.get( ), cmd );
}
bool cylDB::SQliteDepository::createTab( const QString &tab_name, const QVariantMap &tab_info ) const {
	if( !database || tab_info.isEmpty( ) || tab_name.isEmpty( ) )
		return false;
	QString cmd( R"(CREATE TABLE IF NOT EXISTS `%1` ( )" );
	cmd = cmd.arg( tab_name );
	append_map( cmd, tab_info );
	cmd.append( ");" );
	return is_QSqlQuery_run( database.get( ), cmd );
}
IResultInfo_Shared cylDB::SQliteDepository::getTabInfo( const QString &tab_name ) const {
	if( database ) {
		QString cmd = R"( PRAGMA table_info( `%1` ); )";
		cmd = cmd.arg( tab_name );
		return get_QSqlQuery_run( database.get( ), cmd );
	}
	return nullptr;
}
IResultInfo_Shared SQliteDepository::getAllTab( ) const {
	if( database ) {
		QSqlQuery query( *database );
		auto cmd = R"(SELECT * FROM sqlite_master ;)";
		return get_QSqlQuery_run( database.get( ), cmd );
	}
	return nullptr;
}
bool SQliteDepository::addItem( const QString &tab_name, const QStringList &item_name, const QStringList &item_value ) const {
	if( database ) {
		if( item_name.size( ) == 0 || item_value.size( ) == 0 )
			return false;
		QString cmd = R"(INSERT INTO )";
		cmd.append( tab_name ).append( "( " );
		append_name( cmd, item_name );
		cmd.append( " ) VALUES ( " );
		append_value( cmd, item_value );
		cmd.append( " );" );
		return is_QSqlQuery_run( database.get( ), cmd );
	}

	return false;
}
bool SQliteDepository::addItem( const QString &tab_name, const QStringList &item_name, const QStringList &item_value, const QString &where ) const {
	if( database ) {
		if( item_name.size( ) == 0 || item_value.size( ) == 0 )
			return false;
		if( where.isEmpty( ) )
			return addItem( tab_name, item_name, item_value );

		QString cmd = R"(INSERT INTO `)";
		cmd.append( tab_name ).append( "` ( `" );
		append_name( cmd, item_name );
		cmd.append( " ) VALUES ( " );
		append_value( cmd, item_value );
		cmd.append( " ) WHERE" );
		cmd.append( where );
		cmd.append( ";" );
		return is_QSqlQuery_run( database.get( ), cmd );
	}
	return false;
}
IResultInfo_Shared SQliteDepository::findItems( const QString &tab_name, const QStringList &item_name ) const {
	if( database ) {
		QString cmd = R"(SELECT  )";
		append_name( cmd, item_name );
		cmd.append( R"( FROM )" );
		cmd.append( tab_name );
		cmd.append( R"(;)" );
		return get_QSqlQuery_run( database.get( ), cmd );
	}
	return nullptr;
}
IResultInfo_Shared SQliteDepository::findItems( const QString &tab_name, const QStringList &item_name, const QString &where ) const {
	if( database ) {
		if( where.isEmpty( ) )
			return findItems( tab_name, item_name );
		QString cmd = R"(SELECT  )";
		append_name( cmd, item_name );
		cmd.append( R"( FROM )" );
		cmd.append( tab_name );
		cmd.append( " " );
		cmd.append( where );
		cmd.append( R"(;)" );
		return get_QSqlQuery_run( database.get( ), cmd );
	}
	return nullptr;
}
IResultInfo_Shared SQliteDepository::findItems( const QString &tab_name, const QString &where ) const {
	if( database ) {
		if( where.isEmpty( ) )
			return findItems( tab_name );
		QString cmd = R"(SELECT * FROM )";
		cmd.append( tab_name );
		cmd.append( " WHERE " );
		cmd.append( where );
		cmd.append( " ;" );
		return get_QSqlQuery_run( database.get( ), cmd );
	}
	return nullptr;
}
IResultInfo_Shared SQliteDepository::findItems( const QString &tab_name ) const {
	if( database ) {
		QString cmd = R"(SELECT * FROM )";
		cmd.append( tab_name );
		cmd.append( R"(;)" );
		return get_QSqlQuery_run( database.get( ), cmd );
	}
	return nullptr;
}

bool cylDB::SQliteDepository::removeItem( const QString &tab_name, const QString &item_name ) const {
	return false;
}
bool SQliteDepository::updateItem( const QString &tab_name, QMap< QVariant, QVariant > var_map_s ) const {
	return false;
}
void cylDB::SQliteDepository::setUserInfo( const QString &user, const QString &password ) {

}
bool cylDB::SQliteDepository::isOpen( ) const {
	if( database )
		return database->isOpen( );
	return false;
}
bool cylDB::SQliteDepository::open( ) const {
	if( database ) {
		if( database->isOpen( ) || database->open( ) )
			return true;
		return database->open( user, password );
	}
	return false;
}
bool cylDB::SQliteDepository::open( const QString &user, const QString &password ) {
	if( database )
		if( database->isOpen( ) || database->open( ) || database->open( user, password ) ) {
			this->user = user;
			this->password = password;
			return true;
		}
	return false;
}
bool cylDB::SQliteDepository::close( ) const {
	if( database ) {
		database->close( );
		return true;
	}
	return false;
}
bool cylDB::SQliteDepository::isOpenError( ) const {
	return database->isOpen( );
}
QSqlError cylDB::SQliteDepository::getLastError( ) const {
	return database->lastError( );
}
cylDB::Depository_Shared cylDB::SQliteDepository::createDepository( const QString &db_path ) {
	QFileInfo fileInfo( db_path );
	if( fileInfo.exists( ) && fileInfo.isFile( ) )
		return std::make_shared< SQliteDepository >( db_path );
	auto dbPath = fileInfo.absoluteFilePath( );
	auto linkDatabase = make_QSqlDatabase( db_path );;
	linkDatabase->setDatabaseName( dbPath );
	bool openStatus = linkDatabase->isOpen( );
	if( openStatus )
		return std::shared_ptr< SQliteDepository >( new SQliteDepository( linkDatabase, dbPath ) );

	openStatus = linkDatabase->open( );
	if( openStatus )
		return std::shared_ptr< SQliteDepository >( new SQliteDepository( linkDatabase, dbPath ) );

	return nullptr;
}
cylDB::Depository_Shared cylDB::SQliteDepository::createDepository( const QString &db_path, const QString &user, const QString &password ) {
	QFileInfo fileInfo( db_path );
	if( fileInfo.exists( ) && fileInfo.isFile( ) )
		return std::make_shared< SQliteDepository >( db_path );
	auto linkDatabase = make_QSqlDatabase( db_path );;
	linkDatabase->setDatabaseName( db_path );
	if( !linkDatabase->isOpen( ) && !linkDatabase->open( ) && !linkDatabase->open( user, password ) )
		return nullptr;
	std::shared_ptr< SQliteDepository > qliteDepository( new SQliteDepository( linkDatabase, db_path ) );
	qliteDepository->user = user;
	qliteDepository->password = password;
	return qliteDepository;
}
