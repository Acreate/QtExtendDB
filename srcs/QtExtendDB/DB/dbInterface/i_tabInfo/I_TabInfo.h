#ifndef I_TABINFO_H_H_HEAD__FILE__
#define I_TABINFO_H_H_HEAD__FILE__
#pragma once
#include <qstring.h>

#include "../../../nameSpace/cylDB.h"
namespace cylDB {
	class QTEXTENDDB_EXPORT I_TabInfo {
	public:
		virtual ~I_TabInfo( ) { }
	public: // - 接口
		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		virtual QString getName( ) const = 0;
		/// <summary>
		/// 插入一个标头
		/// </summary>
		/// <param name="vector_var_s">插入的信息来源</param>
		/// <returns></returns>
		virtual bool insterTitle( const Vector_VarSPtr_Shared &vector_var_s ) =0;
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
		virtual bool insterTitle( const Var_Shared &index, const Var_Shared &name, const Var_Shared &type, const Var_Shared &not_null, const Var_Shared &default_value, const Var_Shared &is_key ) = 0;
		/// <summary>
		/// 获取表中指定下标的标头名称
		/// </summary>
		/// <param name="index">下标</param>
		/// <returns>标头名称</returns>
		virtual const Var_Shared getTitleIndexName( const size_t index ) const =0;
		/// <summary>
		/// 获取表中指定下标的标头类型
		/// </summary>
		/// <param name="index">下标</param>
		/// <returns>标头类型</returns>
		virtual const Var_Shared getTitleIndexType( const size_t index ) const =0;
		/// <summary>
		/// 获取表中指定下标的标头可空标识位
		/// </summary>
		/// <param name="index">下标</param>
		/// <returns>可空标识位</returns>
		virtual const Var_Shared getTitleIndexIsNotnull( const size_t index ) const =0;
		/// <summary>
		/// 获取表中指定下标的标头默认值
		/// </summary>
		/// <param name="index">下标</param>
		/// <returns>标头默认值</returns>
		virtual const Var_Shared getTitleIndexDefultValue( const size_t index ) const =0;
		/// <summary>
		/// 获取表中指定下标的标头值是否为主键
		/// </summary>
		/// <param name="index">下标</param>
		/// <returns>标头值是否为主键</returns>
		virtual const Var_Shared getTitleIndexIsPK( const size_t index ) const =0;
		/// <summary>
		/// 获取所有下标
		/// </summary>
		/// <returns>下标列表</returns>
		virtual std::vector< int > getIndexs( ) const = 0;
	};

}


#endif // I_TABINFO_H_H_HEAD__FILE__
