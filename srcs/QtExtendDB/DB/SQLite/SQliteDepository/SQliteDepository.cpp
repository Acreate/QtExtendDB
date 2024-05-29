#include "SQliteDepository.h"
#include "../sqliteResult/SQLiteResult.h"
#include <qfileinfo.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlResult>
#include <QSqlQuery>
#include <QMutex>
#include <QMap>

#include "../sqliteTabInfo/SQLiteTabInfo.h"

using namespace cylDB;

namespace sqlTools {
	/// <summary>
	/// 输出 sqlite 执行的返回
	/// </summary>
	/// <param name="result_info_shared">返回对象指针</param>
	void outDebug( cylDB::IResultInfo_Shared &result_info_shared ) {
		if( result_info_shared ) {
			result_info_shared->resetColIndex( );
			auto sharedPtrs = result_info_shared->getCurrentRows( );
			size_t count = 1;
			if( sharedPtrs->size( ) != 0 ) {
				qDebug( ) << "∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵";
				QStringList buffMsg;
				do {
					for( auto &row : *sharedPtrs )
						buffMsg.append( row->toString( ) );
					qDebug( ) << count << '\t' << buffMsg.join( ", " ).toStdString( ).c_str( );
					bool cond = !result_info_shared->nextCol( );
					if( cond )
						break;
					sharedPtrs = result_info_shared->getCurrentRows( );
					if( sharedPtrs->size( ) == 0 )
						break;
					buffMsg.clear( );
					++count;
				} while( true );
				qDebug( ) << "∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴";
				return;
			}
		}
		qDebug( ) << "\t" << "result_info_shared 值为 nullptr";

	}
	static QMap< QString, size_t > connectionNameCount; // 数据库名称链接计数器
	static QMutex qtMutex; // qt 锁
	/// <summary>
	/// 创建一个 sqlite链接
	/// </summary>
	/// <param name="connectionName">链接名称</param>
	/// <returns>链接</returns>
	inline std::shared_ptr< QSqlDatabase > make_QSqlDatabase( const QString &connectionName ) {
		QMutexLocker lock( &qtMutex );
		/// 释放处理调用
		auto releaseFunction = []( QSqlDatabase *p ) {
			QString connectionName = p->connectionName( );
			if( p->isOpen( ) ) {
				p->commit( );
				p->close( );
			}
			delete p;
			auto iterator = connectionNameCount.begin( );
			auto end = connectionNameCount.end( );
			for( ; iterator != end; ++iterator )
				if( iterator.key( ) == connectionName ) {
					size_t &value = iterator.value( );
					--value;
					if( value == 0 )
						QSqlDatabase::removeDatabase( connectionName );
					break;
				}
		};
		if( QSqlDatabase::contains( connectionName ) ) {
			++connectionNameCount[ connectionName ];
			return std::shared_ptr< QSqlDatabase >( new QSqlDatabase( QSqlDatabase::database( connectionName ) )
				, releaseFunction );
		} else {
			connectionNameCount.insert( connectionName, 1 );
			return std::shared_ptr< QSqlDatabase >( new QSqlDatabase( QSqlDatabase::addDatabase( "QSQLITE", connectionName ) )
				, releaseFunction );
		}

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
	/// 输出错误信息
	/// </summary>
	/// <param name="sql_quert">执行对象</param>
	/// <param name="exe_result">执行返回</param>
	/// <param name="cmd">执行命令行</param>
	/// <param name="file">调用文件</param>
	/// <param name="line">调用行数</param>
	inline void out_SQLResult_error( QSqlQuery &sql_quert, bool exe_result, const QString &cmd, const char *file, const size_t line ) {
		if( !exe_result )
			qDebug( ) << "\n=====\tQSqlQuery 执行错误\n\"" << cmd.toStdString( ).c_str( ) << "\"\n\t( 行:" << line << ") \n\t*" << file << "\n\t:=>" << sql_quert.lastError( ) << "\n=====\n";
	}
	/// <summary>
	/// 输出错误信息
	/// </summary>
	/// <param name="sql_quert">执行对象</param>
	/// <param name="exe_result">执行返回</param>
	/// <param name="file">调用文件</param>
	/// <param name="line">调用行数</param>
	inline void out_SQLResult_error( QSqlQuery &sql_quert, bool exe_result, const char *file, const size_t line ) {
		if( !exe_result )
			qDebug( ) << "\n=====\tQSqlQuery 执行错误\n----\"" << sql_quert.lastQuery( ).toStdString( ).c_str( ) << "\"\n----\n----" << sql_quert.lastQuery( ).toStdString( ).c_str( ) << "\n----\n\t( 行:" << line << ") \n\t*" << file << "\n\t:=>" << sql_quert.lastError( ) << "\n=====\n";
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
		out_SQLResult_error( query, exec, cmd, file, line );
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
		out_SQLResult_error( query, exec, cmd, file, line );
		return nullptr;
	}
	inline Vector_VarSPtr_Shared find_QSqlQuery_run( cylDB::IResultInfo_Shared &result_info_shared, std::function< bool( const size_t, const Vector_VarSPtr_Shared & ) > call_function ) {
		auto element = result_info_shared.get( );
		if( !element )
			return nullptr;
		// 解析表明
		element->resetColIndex( );
		auto currentRows = element->getCurrentRows( );
		if( currentRows->size( ) > 0 )
			do {
				if( call_function( element->currentCol( ), currentRows ) )
					return currentRows;
				if( !element->nextCol( ) )
					break;
				currentRows = element->getCurrentRows( );
				if( currentRows->size( ) == 0 )
					break;
			} while( true );
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
	if( !isTransaction ) {
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
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
	if( !isTransaction ) {
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
}
bool SQliteDepository::removeTab( const QString &tab_name ) const {
	auto element = database.get( );
	if( !element )
		return false;
	QString cmd( R"(DROP TABLE IF EXISTS `%1`;)" );
	cmd = cmd.arg( tab_name );
	if( !isTransaction ) {
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
}
IResultInfo_Shared cylDB::SQliteDepository::getTabInfo( const QString &tab_name ) const {
	QSqlDatabase *element = database.get( );
	if( !element )
		return nullptr;
	QString cmd = R"( PRAGMA table_info( `%1` ); )";
	cmd = cmd.arg( tab_name );
	if( isTransaction ) {
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
}
IResultInfo_Shared SQliteDepository::getAllTab( ) const {
	QSqlDatabase *element = database.get( );
	if( !element )
		return nullptr;
	QSqlQuery query( *database );
	auto cmd = R"(SELECT * FROM sqlite_master ;)";

	if( isTransaction ) {
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );

}
bool SQliteDepository::addItem( const QString &tab_name, const QStringList &item_name, const QStringList &item_value ) const {
	if( database ) {
		QSqlDatabase *element = database.get( );
		if( !element || item_name.size( ) == 0 || item_value.size( ) == 0 )
			return false;
		QString cmd = R"(INSERT INTO `)";
		cmd.append( tab_name ).append( "`( " );
		sqlTools::append_name( cmd, item_name );
		cmd.append( " ) VALUES ( " );
		sqlTools::append_value( cmd, item_value );
		cmd.append( " );" );
		if( !isTransaction ) {
			QMutexLocker< QMutex > locker( dbMutex.get( ) );
			return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
		}
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
		if( !isTransaction ) {
			QMutexLocker< QMutex > locker( dbMutex.get( ) );
			return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
		}
		return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return false;
}
IResultInfo_Shared SQliteDepository::findItems( const QString &tab_name, const QStringList &item_name ) const {
	QSqlDatabase *element = database.get( );
	if( element ) {
		QString cmd = R"(SELECT  )";
		sqlTools::append_name( cmd, item_name );
		cmd.append( R"( FROM `)" );
		cmd.append( tab_name );
		cmd.append( R"(`;)" );
		if( isTransaction ) {
			QMutexLocker< QMutex > locker( dbMutex.get( ) );
			return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
		}
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
		cmd.append( R"( FROM `)" );
		cmd.append( tab_name );
		cmd.append( "` " );
		cmd.append( where );
		cmd.append( R"(;)" );
		if( isTransaction ) {
			QMutexLocker< QMutex > locker( dbMutex.get( ) );
			return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
		}
		return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return nullptr;
}
IResultInfo_Shared SQliteDepository::findItems( const QString &tab_name, const QString &where ) const {
	QSqlDatabase *element = database.get( );
	if( element ) {
		if( where.isEmpty( ) )
			return findItems( tab_name );
		QString cmd = R"(SELECT * FROM `)";
		cmd.append( tab_name );
		cmd.append( "` WHERE " );
		cmd.append( where );
		cmd.append( " ;" );
		if( isTransaction ) {
			QMutexLocker< QMutex > locker( dbMutex.get( ) );
			return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
		}
		return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return nullptr;
}
IResultInfo_Shared SQliteDepository::findItems( const QString &tab_name ) const {
	QSqlDatabase *element = database.get( );
	if( element ) {
		QString cmd = R"(SELECT * FROM `)";
		cmd.append( tab_name );
		cmd.append( R"(`;)" );
		if( isTransaction ) {
			QMutexLocker< QMutex > locker( dbMutex.get( ) );
			return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
		}
		return sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );

	}
	return nullptr;
}


bool SQliteDepository::removeItem( const QString &tab_name, const QString &where ) const {
	QSqlDatabase *element = database.get( );
	if( !element || where.isEmpty( ) )
		return false;
	QString cmd = QString( "DELETE FROM `" ) + tab_name + "` WHERE " + where + " ;";
	if( !isTransaction ) {
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
}
bool SQliteDepository::removeItem( const QString &tab_name ) const {
	QSqlDatabase *element = database.get( );
	if( !element )
		return false;
	QString cmd = QString( "DELETE FROM `" ) + tab_name + "` ;";
	if( !isTransaction ) {
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
}
bool SQliteDepository::updateItem( const QString &tab_name, const QVariantMap &var_map_s ) const {
	QSqlDatabase *element = database.get( );
	if( !element || var_map_s.isEmpty( ) )
		return false;
	QString cmd = "UPDATE `";
	cmd.append( tab_name ).append( "` SET " );
	sqlTools::append_map_update( cmd, var_map_s );
	cmd.append( " ;" );
	if( !isTransaction ) {
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
	return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
}
ITabInfo_Shared SQliteDepository::converTab( const QString &tab_name ) const {
	QSqlDatabase *element = database.get( );
	if( !element )
		return nullptr;
	QString cmd = R"(SELECT * FROM sqlite_master ;)";
	IResultInfo_Shared qSqlQueryRun;
	if( isTransaction )
		qSqlQueryRun = sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	else {
		dbMutex->lock( );
		qSqlQueryRun = sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
		dbMutex->unlock( );
	}

	if( qSqlQueryRun ) {
		// 解析表明
		auto currentRows = sqlTools::find_QSqlQuery_run( qSqlQueryRun
			, [&]( const size_t index, const Vector_VarSPtr_Shared &vector_var_s )->bool {
				if( vector_var_s->at( 1 )->toString( ) == tab_name || vector_var_s->at( 2 )->toString( ) == tab_name )
					return true;
				return false;
			} );
		std::vector< std::shared_ptr< QVariant > > *extent = currentRows.get( );
		if( extent ) { // 获取表信息
			ITabInfo_Shared result( std::make_shared< SQLiteTabInfo >( tab_name ) );
			cmd = R"( PRAGMA table_info( `%1` ); )";
			cmd = cmd.arg( tab_name );
			if( isTransaction )
				qSqlQueryRun = sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
			else {
				dbMutex->lock( );
				qSqlQueryRun = sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
				dbMutex->unlock( );;
			}

			currentRows = sqlTools::find_QSqlQuery_run( qSqlQueryRun
				, [&]( const size_t index, const Vector_VarSPtr_Shared &vector_var_s )->bool {
					QStringList msgList;
					if( vector_var_s->size( ) != 6 )
						return false;
					result->insterTitle( vector_var_s->at( 0 ), vector_var_s->at( 1 ), vector_var_s->at( 2 ), vector_var_s->at( 3 ), vector_var_s->at( 4 ), vector_var_s->at( 5 ) );
					return false;
				} );
			return result;
		}
	}
	return nullptr;
}
Vector_ITabInfoSPtr_Shared SQliteDepository::converAllTab( ) const {
	QSqlDatabase *element = database.get( );
	if( !element )
		return nullptr;
	QString cmd = R"(SELECT * FROM sqlite_master ;)";
	IResultInfo_Shared qSqlQueryRun;
	if( isTransaction )
		qSqlQueryRun = sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	else {
		dbMutex->lock( );
		qSqlQueryRun = sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
		dbMutex->unlock( );
	}
	if( qSqlQueryRun ) {
		Vector_ITabInfoSPtr_Shared resultVector( std::make_shared< Vector_ITabInfoSPtr >( ) );
		// 解析表明
		sqlTools::find_QSqlQuery_run( qSqlQueryRun
			, [&]( const size_t index, const Vector_VarSPtr_Shared &vector_var_s )->bool {
				auto tabName = vector_var_s->at( 2 )->toString( );
				ITabInfo_Shared result( std::make_shared< SQLiteTabInfo >( tabName ) );
				resultVector->emplace_back( result );
				cmd = R"( PRAGMA table_info( `%1` ); )";
				cmd = cmd.arg( tabName );
				if( isTransaction ) {
					auto SubqSqlQueryRun = sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
					sqlTools::find_QSqlQuery_run( SubqSqlQueryRun
						, [&]( const size_t sub_index, const Vector_VarSPtr_Shared &sub_vector_var_s )->bool {
							result->insterTitle( sub_vector_var_s );
							return false;
						} );
				} else {
					dbMutex->lock( );
					auto SubqSqlQueryRun = sqlTools::get_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
					dbMutex->unlock( );;
					sqlTools::find_QSqlQuery_run( SubqSqlQueryRun
						, [&]( const size_t sub_index, const Vector_VarSPtr_Shared &sub_vector_var_s )->bool {
							result->insterTitle( sub_vector_var_s );
							return false;
						} );
				}
				return false;
			} );
		return resultVector;
	}
	return nullptr;
}
bool SQliteDepository::exec( const QString &cmd, IResultInfo_Shared &sql_exec_result ) const {
	QSqlDatabase *element = database.get( );
	if( element == nullptr )
		return false;
	QSqlQuery query( *database );

	if( isTransaction ) {
		bool exec = query.exec( cmd );
		sqlTools::out_SQLResult_error( query, exec, cmd, __FILE__, __LINE__ );
		sql_exec_result = sqlTools::get_QSqlQuery_result( query );
		return exec;
	} else {
		QMutexLocker< QMutex > loco( dbMutex.get( ) );
		bool exec = query.exec( cmd );
		sqlTools::out_SQLResult_error( query, exec, cmd, __FILE__, __LINE__ );
		sql_exec_result = sqlTools::get_QSqlQuery_result( query );
		return exec;
	}
}
bool SQliteDepository::exec( QSqlQuery *query, IResultInfo_Shared &sql_exec_result ) const {
	if( isTransaction ) {
		bool exec = query->exec( );
		sqlTools::out_SQLResult_error( *query, exec, query->lastQuery( ), __FILE__, __LINE__ );
		sql_exec_result = sqlTools::get_QSqlQuery_result( *query );
		return exec;
	} else {
		QMutexLocker< QMutex > loco( dbMutex.get( ) );
		bool exec = query->exec( );
		sqlTools::out_SQLResult_error( *query, exec, query->lastQuery( ), __FILE__, __LINE__ );
		sql_exec_result = sqlTools::get_QSqlQuery_result( *query );
		return exec;
	}
}
bool SQliteDepository::exec( QSqlQuery *query ) const {
	if( isTransaction ) {
		bool exec = query->exec( );
		sqlTools::out_SQLResult_error( *query, exec, __FILE__, __LINE__ );
		return exec;
	} else {
		QMutexLocker< QMutex > loco( dbMutex.get( ) );
		bool exec = query->exec( );
		sqlTools::out_SQLResult_error( *query, exec, __FILE__, __LINE__ );
		return exec;
	}
}
bool SQliteDepository::updateItem( const QString &tab_name, const QVariantMap &var_map_s, const QString &where ) const {

	if( where.isEmpty( ) )
		return updateItem( tab_name, var_map_s );
	QSqlDatabase *element = database.get( );
	if( !element || var_map_s.isEmpty( ) )
		return false;
	QString cmd = "UPDATE `";
	cmd.append( tab_name ).append( "` SET " );
	sqlTools::append_map_update( cmd, var_map_s );
	cmd.append( " WHERE " );
	cmd.append( where );
	cmd.append( " ;" );
	if( !isTransaction ) {
		QMutexLocker< QMutex > locker( dbMutex.get( ) );
		return sqlTools::is_QSqlQuery_run( element, cmd, __FILE__, __LINE__ );
	}
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
		database->commit( );
		return !database->isOpen( );
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
