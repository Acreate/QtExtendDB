#ifndef SQLITETABINFO_H_H_HEAD__FILE__
#define SQLITETABINFO_H_H_HEAD__FILE__
#pragma once

#include "../../../nameSpace/cylDB.h"
#include "../../dbInterface/i_tabInfo/I_TabInfo.h"
#include <qdatetime.h>
namespace cylDB {
	class QTEXTENDDB_EXPORT SQLiteTabInfo : public I_TabInfo {
		using Release_Call = std::function< void( QSqlDatabase **database_ptr ) >;
		friend class SQliteDepository;
		using Tab_Title_Info = struct Tab_Title_Info {
			Var_Shared index; // 位置
			Var_Shared name; // 名称
			Var_Shared type; // 类型
			Var_Shared not_null; // 是否允许是 null
			Var_Shared default_value; // 默认值
			Var_Shared is_key; // 是否主键
			/// <summary>
			/// 创建一个标头信息
			/// </summary>
			/// <param name="index">位置</param>
			/// <param name="name">名称</param>
			/// <param name="type">类型</param>
			/// <param name="not_null">是否允许是 null</param>
			/// <param name="default_value">默认值</param>
			/// <param name="is_key">是否主键</param>
			Tab_Title_Info( const Var_Shared &index, const Var_Shared &name, const Var_Shared &type, const Var_Shared &not_null, const Var_Shared &default_value, const Var_Shared &is_key )
				: index( index ),
				name( name ),
				type( type ),
				not_null( not_null ),
				default_value( default_value ),
				is_key( is_key ) { }
		};
	private: // - 私有成员
		QString name; // 表名称
		std::vector< Tab_Title_Info > titleVector;
	public: // - 构造函数
		explicit SQLiteTabInfo( const QString &name )
			: name( name ) { }
		~SQLiteTabInfo( ) override {
		}
		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		QString getName( ) const override {
			return name;
		}
		/// <summary>
		/// 插入一个标头
		/// </summary>
		/// <param name="vector_var_s">插入的信息来源</param>
		/// <returns></returns>
		bool insterTitle( const Vector_VarSPtr_Shared &vector_var_s ) override {
			if( vector_var_s )
				if( vector_var_s->size( ) == 6 )
					return insterTitle( vector_var_s->at( 0 ), vector_var_s->at( 1 ), vector_var_s->at( 2 ), vector_var_s->at( 3 ), vector_var_s->at( 4 ), vector_var_s->at( 5 ) );
			return false;
		}

		/// <summary>
		/// 插入一个标头
		/// </summary>
		/// <param name="index">标头位于表的下标</param>
		/// <param name="name">标头名称</param>
		/// <param name="type">标头类型</param>
		/// <param name="not_null">标头下的值是否允许等于 nullptr</param>
		/// <param name="default_value">标头的默认值</param>
		/// <param name="is_key">是否作为表的主键</param>
		/// <returns>成功返回 true</returns>
		bool insterTitle( const Var_Shared &index, const Var_Shared &name, const Var_Shared &type, const Var_Shared &not_null, const Var_Shared &default_value, const Var_Shared &is_key ) override {
			auto iterator = titleVector.begin( );
			auto end = titleVector.end( );
			for( ; iterator != end; ++iterator )
				if( *iterator->index.get( ) == *index.get( ) ) // 比较下标，如果存在，则返回false
					return false;
			titleVector.emplace_back( Tab_Title_Info( index, name, type, not_null, default_value, is_key ) );
			return true;
		}
		/// <summary>
		/// 获取所有下标
		/// </summary>
		/// <returns>下标列表</returns>
		std::vector< int > getIndexs( ) const override {
			std::vector< int > result;
			for( auto &var : titleVector )
				result.emplace_back( var.index->toInt( ) );
			return result;
		}
		
		/// <summary>
		/// 获取表中指定下标的标头名称
		/// </summary>
		/// <param name="index">下标</param>
		/// <returns>标头名称</returns>
		const Var_Shared getTitleIndexName( const size_t index ) const override {
			for( auto &var : titleVector )
				if( *var.index == index )
					return var.name;
			return nullptr;
		}
		/// <summary>
		/// 获取表中指定下标的标头类型
		/// </summary>
		/// <param name="index">下标</param>
		/// <returns>标头类型</returns>
		const Var_Shared getTitleIndexType( const size_t index ) const override {
			for( auto &var : titleVector )
				if( *var.index == index )
					return var.type;
			return nullptr;
		}
		/// <summary>
		/// 获取表中指定下标的标头可空标识位
		/// </summary>
		/// <param name="index">下标</param>
		/// <returns>可空标识位</returns>
		const Var_Shared getTitleIndexIsNotnull( const size_t index ) const override {
			for( auto &var : titleVector )
				if( *var.index == index )
					return var.not_null;
			return nullptr;
		}
		/// <summary>
		/// 获取表中指定下标的标头默认值
		/// </summary>
		/// <param name="index">下标</param>
		/// <returns>标头默认值</returns>
		const Var_Shared getTitleIndexDefultValue( const size_t index ) const override {
			for( auto &var : titleVector )
				if( *var.index == index )
					return var.default_value;
			return nullptr;
		}
		/// <summary>
		/// 获取表中指定下标的标头值是否为主键
		/// </summary>
		/// <param name="index">下标</param>
		/// <returns>标头值是否为主键</returns>
		const Var_Shared getTitleIndexIsPK( const size_t index ) const override {
			for( auto &var : titleVector )
				if( *var.index == index )
					return var.is_key;
			return nullptr;
		}
	};
}


#endif // SQLITETABINFO_H_H_HEAD__FILE__
