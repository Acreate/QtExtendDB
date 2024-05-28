#include "SQliteDepository.h"
#include "../sqliteResult/SQLiteResult.h"
#include <qfileinfo.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlResult>
#include <QSqlQuery>
#include <QMutex>
#include <QMap>

using namespace cylDB;

namespace sqlTools {

	/// <summary>
	/// 创建一个 sqlite链接
	/// </summary>
	/// <param name="connectionName">链接名称</param>
	/// <returns>链接</returns>
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
	/// <summary>
	/// 获得 QSqlQuery 的执行返回结果
	/// </summary>
	/// <param name="q_sql_result">查询对象</param>
	/// <returns>失败返回 nullptr</returns>
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
	/// <summary>
	/// 执行一条命令，并且返回是否执行成功
	/// </summary>
	/// <param name="database">执行数据库</param>
	/// <param name="cmd">执行命令行</param>
	/// <param name="file">调用文件名</param>
	/// <param name="line">调用行</param>
	/// <returns>成功返回 true</returns>
	inline bool is_QSqlQuery_run( const QSqlDatabase *database, const QString &cmd, const char *file, const size_t line ) {

		QSqlQuery query( *database );
		bool exec = query.exec( cmd );
		if( !exec )
			qDebug( ) << cmd << "( 行:" << line << ") \n\t*" << file << ": " << query.lastError( );
		return exec;
	}
	/// <summary>
	/// 执行一条命令，并且返回是否执行成功
	/// </summary>
	/// <param name="database">执行数据库</param>
	/// <param name="cmd">执行命令行</param>
	/// <param name="file">调用文件名</param>
	/// <param name="line">调用行</param>
	/// <returns>成功返回 true</returns>
	inline bool is_QSqlQuery_run( const std::shared_ptr< QSqlDatabase > &database, const QString &cmd, const char *file, const size_t line ) {
		return is_QSqlQuery_run( database.get( ), cmd, file, line );
	}
	/// <summary>
	/// 执行一条命令，并且返回执行结果
	/// </summary>
	/// <param name="database">执行数据库</param>
	/// <param name="cmd">执行命令行</param>
	/// <param name="file">调用文件名</param>
	/// <param name="line">调用行</param>
	/// <returns>失败返回 nullptr</returns>
	inline IResultInfo_Shared get_QSqlQuery_run( QSqlDatabase *database, const QString &cmd, const char *file, const size_t line ) {
		QSqlQuery query( *database );
		bool exec = query.exec( cmd );
		if( exec )
			return get_QSqlQuery_result( query );
		qDebug( ) << cmd << "( 行:" << line << ") \n\t*" << file << ": " << query.lastError( );
		return nullptr;
	}
	/// <summary>
	/// 表格命令行拼接
	/// </summary>
	/// <param name="cmd">拼接起始</param>
	/// <param name="append_map">拼接对象</param>
	/// <returns>拼接个数</returns>
	inline qsizetype append_map_create_tab( QString &cmd, const QVariantMap &append_map ) {
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
	/// <summary>
	/// 更新命令拼接
	/// </summary>
	/// <param name="cmd">起始拼接对象</param>
	/// <param name="append_map">拼接对象映射</param>
	/// <returns>拼接个数</returns>
	inline qsizetype append_map_update( QString &cmd, const QVariantMap &append_map ) {
		qsizetype result = 0;
		auto iterator = append_map.begin( );
		auto end = append_map.end( );
		do {
			cmd.append( " `" );
			cmd.append( iterator.key( ) );
			cmd.append( "` =" );
			cmd.append( iterator.value( ).toString( ) );
			++iterator;
			++result;
			if( iterator == end )
				break;
			cmd.append( ", " );
		} while( true );

		return result;
	}
	/// <summary>
	/// 名称拼接
	/// </summary>
	/// <param name="cmd">起始拼接对象</param>
	/// <param name="append_list">拼接列表</param>
	/// <returns>拼接个数</returns>
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
	/// <summary>
	/// 值拼接
	/// </summary>
	/// <param name="cmd">起始拼接对象</param>
	/// <param name="append_list">拼接列表</param>
	/// <returns>拼接个数</returns>
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
}

cylDB::SQliteDepository::SQliteDepository( const QString &link_path, const QString &user, const QString &password ) {

	QFileInfo fileInfo( link_path );
	linkPath = fileInfo.absoluteFilePath( );
	this->user = user;
	this->password = password;
	this->dbMutex = std::make_shared< QMutex >( );

	if( fileInfo.exists( ) && fileInfo.isFile( ) ) {
		database = sqlTools::make_QSqlDatabase( linkPath );
		database->setDatabaseName( linkPath );
	} else
		database = nullptr;
}
cylDB::SQliteDepository::SQliteDepository( const QString &link_path ) {
	QFileInfo fileInfo( link_path );
	this->dbMutex = std::make_shared< QMutex >( );
	if( fileInfo.exists( ) && fileInfo.isFile( ) ) {
		linkPath = fileInfo.absoluteFilePath( );
		database = sqlTools::make_QSqlDatabase( linkPath );
		database->setDatabaseName( linkPath );
	}
}
cylDB::SQliteDepository::~SQliteDepository( ) {
}
cylDB::SQliteDepository::SQliteDepository( const std::shared_ptr< QSqlDatabase > &database, const QString &db_path ) {
	this->database = database;
	linkPath = db_path;
	this->dbMutex = std::make_shared< QMutex >( );
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
	if( key.isEmpty( ) )
		return createTab( tab_name, tab_info );
	QSqlDatabase *element = database.get( );
	if( !element || tab_info.isEmpty( ) || tab_name.isEmpty( ) )
		return false;

	QString cmd( R"(CREATE TABLE IF NOT EXISTS `%1` ( )" );
	cmd = cmd.arg( tab_name );

	cmd.append( '`' );
	cmd.append( key );
	cmd.append( "` " );
	cmd.append( key_type );
	cmd.append( "  PRIMARY KEY AUTOINCREMENT, " );
	sqlTools::append_map_create_tab( cmd, tab_info );
	cmd.append( u8" ); " );
	QMutexLocker< QMutex > locker( dbMutex.get( ) );
	return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
}
bool cylDB::SQliteDepository::createTab( const QString &tab_name, const QVariantMap &tab_info ) const {
	QSqlDatabase *element = database.get( );
	if( !element || tab_info.isEmpty( ) || tab_name.isEmpty( ) )
		return false;
	QString cmd( R"(CREATE TABLE IF NOT EXISTS `%1` ( )" );
	cmd = cmd.arg( tab_name );
	sqlTools::append_map_create_tab( cmd, tab_info );
	cmd.append( ");" );
	QMutexLocker< QMutex > locker( dbMutex.get( ) );
	return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
}
bool SQliteDepository::removeTab( const QString &tab_name ) const {
	auto element = database.get( );
	if( !element )
		return false;
	QString cmd( R"(DROP TABLE IF EXISTS `%1`;)" );
	cmd = cmd.arg( tab_name );
	QMutexLocker< QMutex > locker( dbMutex.get( ) );
	return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
}
IResultInfo_Shared cylDB::SQliteDepository::getTabInfo( const QString &tab_name ) const {
	QSqlDatabase *element = database.get( );
	if( !element )
		return nullptr;
	QString cmd = R"( PRAGMA table_info( `%1` ); )";
	cmd = cmd.arg( tab_name );
	QMutexLocker< QMutex > locker( dbMutex.get( ) );
	return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
}
IResultInfo_Shared SQliteDepository::getAllTab( ) const {
	QSqlDatabase *element = database.get( );
	if( !element )
		return nullptr;
	QSqlQuery query( *database );
	auto cmd = R"(SELECT * FROM sqlite_master ;)";
	QMutexLocker< QMutex > locker( dbMutex.get( ) );
	return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );

}
bool SQliteDepository::addItem( const QString &tab_name, const QStringList &item_name, const QStringList &item_value ) const {
	if( database ) {
		QSqlDatabase *element = database.get( );
		if( !element || item_name.size( ) == 0 || item_value.size( ) == 0 )
			return false;
		QString cmd = R"(INSERT INTO )";
		cmd.append( tab_name ).append( "( " );
		sqlTools::append_name( cmd, item_name );
		cmd.append( " ) VALUES ( " );
		sqlTools::append_value( cmd, item_value );
		cmd.append( " );" );
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}

	return false;
}
bool SQliteDepository::addItem( const QString &tab_name, const QStringList &item_name, const QStringList &item_value, const QString &where ) const {
	if( where.isEmpty( ) )
		return addItem( tab_name, item_name, item_value );

	QSqlDatabase *element = database.get( );
	if( element ) {
		if( item_name.size( ) == 0 || item_value.size( ) == 0 )
			return false;
		QString cmd = R"(INSERT INTO `)";
		cmd.append( tab_name ).append( "` ( `" );
		sqlTools::append_name( cmd, item_name );
		cmd.append( " ) VALUES ( " );
		sqlTools::append_value( cmd, item_value );
		cmd.append( " ) WHERE" );
		cmd.append( where );
		cmd.append( ";" );
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return false;
}
IResultInfo_Shared SQliteDepository::findItems( const QString &tab_name, const QStringList &item_name ) const {
	QSqlDatabase *element = database.get( );
	if( element ) {
		QString cmd = R"(SELECT  )";
		sqlTools::append_name( cmd, item_name );
		cmd.append( R"( FROM )" );
		cmd.append( tab_name );
		cmd.append( R"(;)" );
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return nullptr;
}
IResultInfo_Shared SQliteDepository::findItems( const QString &tab_name, const QStringList &item_name, const QString &where ) const {
	QSqlDatabase *element = database.get( );
	if( element ) {
		if( where.isEmpty( ) )
			return findItems( tab_name, item_name );
		QString cmd = R"(SELECT  )";
		sqlTools::append_name( cmd, item_name );
		cmd.append( R"( FROM )" );
		cmd.append( tab_name );
		cmd.append( " " );
		cmd.append( where );
		cmd.append( R"(;)" );
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return nullptr;
}
IResultInfo_Shared SQliteDepository::findItems( const QString &tab_name, const QString &where ) const {
	QSqlDatabase *element = database.get( );
	if( element ) {
		if( where.isEmpty( ) )
			return findItems( tab_name );
		QString cmd = R"(SELECT * FROM )";
		cmd.append( tab_name );
		cmd.append( " WHERE " );
		cmd.append( where );
		cmd.append( " ;" );
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return nullptr;
}
IResultInfo_Shared SQliteDepository::findItems( const QString &tab_name ) const {
	QSqlDatabase *element = database.get( );
	if( element ) {
		QString cmd = R"(SELECT * FROM )";
		cmd.append( tab_name );
		cmd.append( R"(;)" );
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return nullptr;
}


bool SQliteDepository::removeItem( const QString &tab_name, const QString &where ) const {
	QSqlDatabase *element = database.get( );
	if( !element || where.isEmpty( ) )
		return false;
	QString cmd = QString( "DELETE FROM " ) + tab_name + " WHERE " + where + " ;";
	QMutexLocker< QMutex > locker( dbMutex.get( ) );
	return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
}
bool SQliteDepository::removeItem( const QString &tab_name ) const {
	QSqlDatabase *element = database.get( );
	if( !element )
		return false;
	QString cmd = QString( "DELETE FROM " ) + tab_name + " ;";
	QMutexLocker< QMutex > locker( dbMutex.get( ) );
	return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
}
bool SQliteDepository::updateItem( const QString &tab_name, const QVariantMap &var_map_s ) const {
	QSqlDatabase *element = database.get( );
	if( !element || var_map_s.isEmpty( ) )
		return false;
	QString cmd = "UPDATE ";
	cmd.append( tab_name ).append( " SET " );
	sqlTools::append_map_update( cmd, var_map_s );
	cmd.append( " ;" );
	QMutexLocker< QMutex > locker( dbMutex.get( ) );
	return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
}
// todo : 未实现
ITabInfo_Shared SQliteDepository::converTab( const QString &tab_name ) const {
	QSqlDatabase *element = database.get( );
	if( !element )
		return nullptr;
	QString cmd = R"( PRAGMA table_info( `%1` ); )";
	cmd = cmd.arg( tab_name );
	dbMutex->lock( );
	auto qSqlQueryRun = sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	dbMutex->unlock( );
	if( qSqlQueryRun ) {
		// 解析表明
		qSqlQueryRun->resetColIndex( );
		auto currentRows = qSqlQueryRun->getCurrentRows( );
		bool nontFind = true;
		if( currentRows->size( ) > 0 )
			do {
				if( !qSqlQueryRun->nextCol( ) )
					break;
				currentRows = qSqlQueryRun->getCurrentRows( );
				if( currentRows->size( ) == 0 )
					break;
				if( currentRows->at( 2 )->toString( ) == tab_name || currentRows->at( 3 )->toString( ) == tab_name ) {
					nontFind = false;
					break; // 找到
				}
			} while( nontFind );
		if( !nontFind ) {
			cmd = R"( PRAGMA table_info( `%1` ); )";
			cmd = cmd.arg( tab_name );
			dbMutex->lock( );
			qSqlQueryRun = sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
			dbMutex->unlock( );;
			if(qSqlQueryRun) { // 存在返回值
				
			}
		}
	}
	return nullptr;
}
// todo : 未实现
Vector_ITabInfoSPtr_Shared SQliteDepository::converAllTab( ) const {
	return nullptr;
}
bool SQliteDepository::updateItem( const QString &tab_name, const QVariantMap &var_map_s, const QString &where ) const {

	if( where.isEmpty( ) )
		return updateItem( tab_name, var_map_s );
	QSqlDatabase *element = database.get( );
	if( !element || var_map_s.isEmpty( ) )
		return false;
	QString cmd = "UPDATE ";
	cmd.append( tab_name ).append( " SET " );
	sqlTools::append_map_update( cmd, var_map_s );
	cmd.append( " WHERE " );
	cmd.append( where );
	cmd.append( " ;" );
	QMutexLocker< QMutex > locker( dbMutex.get( ) );
	return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
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
	auto linkDatabase = sqlTools::make_QSqlDatabase( db_path );;
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
	auto linkDatabase = sqlTools::make_QSqlDatabase( db_path );;
	linkDatabase->setDatabaseName( db_path );
	if( !linkDatabase->isOpen( ) && !linkDatabase->open( ) && !linkDatabase->open( user, password ) )
		return nullptr;
	std::shared_ptr< SQliteDepository > qliteDepository( new SQliteDepository( linkDatabase, db_path ) );
	qliteDepository->user = user;
	qliteDepository->password = password;
	return qliteDepository;
}
