/*****************************************************************************

Copyright (c) 1996, 2016, Oracle and/or its affiliates. All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Suite 500, Boston, MA 02110-1335 USA

*****************************************************************************/

/**************************************************//**
@file include/dict0crea.h
Database object creation

Created 1/8/1996 Heikki Tuuri
*******************************************************/

#ifndef dict0crea_h
#define dict0crea_h

#include "univ.i"
#include "dict0types.h"
#include "dict0dict.h"
#include "que0types.h"
#include "row0types.h"
#include "mtr0mtr.h"

/*********************************************************************//**
Creates a table create graph.
@return	own: table create node */
UNIV_INTERN
tab_node_t*
tab_create_graph_create(
/*====================*/
	dict_table_t*	table,	/*!< in: table to create, built as a memory data
				structure */
	mem_heap_t*	heap,	/*!< in: heap where created */
	bool		commit);/*!< in: true if the commit node should be
				added to the query graph */
/*********************************************************************//**
Creates an index create graph.
@return	own: index create node */
UNIV_INTERN
ind_node_t*
ind_create_graph_create(
/*====================*/
	dict_index_t*	index,	/*!< in: index to create, built as a memory data
				structure */
	mem_heap_t*	heap,	/*!< in: heap where created */
	bool		commit);/*!< in: true if the commit node should be
				added to the query graph */
/***********************************************************//**
Creates a table. This is a high-level function used in SQL execution graphs.
@return	query thread to run next or NULL */
UNIV_INTERN
que_thr_t*
dict_create_table_step(
/*===================*/
	que_thr_t*	thr);	/*!< in: query thread */
/***********************************************************//**
Creates an index. This is a high-level function used in SQL execution
graphs.
@return	query thread to run next or NULL */
UNIV_INTERN
que_thr_t*
dict_create_index_step(
/*===================*/
	que_thr_t*	thr);	/*!< in: query thread */
/*******************************************************************//**
Truncates the index tree associated with a row in SYS_INDEXES table.
@return	new root page number, or FIL_NULL on failure */
UNIV_INTERN
ulint
dict_truncate_index_tree(
/*=====================*/
	dict_table_t*	table,	/*!< in: the table the index belongs to */
	ulint		space,	/*!< in: 0=truncate,
				nonzero=create the index tree in the
				given tablespace */
	btr_pcur_t*	pcur,	/*!< in/out: persistent cursor pointing to
				record in the clustered index of
				SYS_INDEXES table. The cursor may be
				repositioned in this call. */
	mtr_t*		mtr);	/*!< in: mtr having the latch
				on the record page. The mtr may be
				committed and restarted in this call. */
/*******************************************************************//**
Drops the index tree associated with a row in SYS_INDEXES table. */
UNIV_INTERN
void
dict_drop_index_tree(
/*=================*/
	rec_t*	rec,	/*!< in/out: record in the clustered index
			of SYS_INDEXES table */
	mtr_t*	mtr);	/*!< in: mtr having the latch on the record page */
/****************************************************************//**
Creates the foreign key constraints system tables inside InnoDB
at server bootstrap or server start if they are not found or are
not of the right form.
@return	DB_SUCCESS or error code */
UNIV_INTERN
dberr_t
dict_create_or_check_foreign_constraint_tables(void);
/*================================================*/

/********************************************************************//**
Construct foreign key constraint defintion from data dictionary information.
*/
UNIV_INTERN
char*
dict_foreign_def_get(
/*=================*/
	dict_foreign_t*	foreign,/*!< in: foreign */
	trx_t*		trx);	/*!< in: trx */

/********************************************************************//**
Generate a foreign key constraint name when it was not named by the user.
A generated constraint has a name of the format dbname/tablename_ibfk_NUMBER,
where the numbers start from 1, and are given locally for this table, that is,
the number is not global, as it used to be before MySQL 4.0.18.  */
UNIV_INLINE
dberr_t
dict_create_add_foreign_id(
/*=======================*/
	ulint*		id_nr,	/*!< in/out: number to use in id generation;
				incremented if used */
	const char*	name,	/*!< in: table name */
	dict_foreign_t*	foreign)/*!< in/out: foreign key */
	MY_ATTRIBUTE((nonnull));

/** Adds the given set of foreign key objects to the dictionary tables
in the database. This function does not modify the dictionary cache. The
caller must ensure that all foreign key objects contain a valid constraint
name in foreign->id.
@param[in]	local_fk_set	set of foreign key objects, to be added to
the dictionary tables
@param[in]	table		table to which the foreign key objects in
local_fk_set belong to
@param[in,out]	trx		transaction
@return error code or DB_SUCCESS */
UNIV_INTERN
dberr_t
dict_create_add_foreigns_to_dictionary(
/*===================================*/
	const dict_foreign_set&	local_fk_set,
	const dict_table_t*	table,
	trx_t*			trx)
	MY_ATTRIBUTE((nonnull, warn_unused_result));
/****************************************************************//**
Creates the tablespaces and datafiles system tables inside InnoDB
at server bootstrap or server start if they are not found or are
not of the right form.
@return	DB_SUCCESS or error code */
UNIV_INTERN
dberr_t
dict_create_or_check_sys_tablespace(void);
/*=====================================*/

#define ZIP_DICT_MAX_NAME_LENGTH 64
/* Max window size (2^15) minus 262 */
#define ZIP_DICT_MAX_DATA_LENGTH 32506

/** Creates the zip_dict system table inside InnoDB
at server bootstrap or server start if it is not found or is
not of the right form.
@return	DB_SUCCESS or error code */
UNIV_INTERN
dberr_t
dict_create_or_check_sys_zip_dict(void);

/********************************************************************//**
Add a single tablespace definition to the data dictionary tables in the
database.
@return	error code or DB_SUCCESS */
UNIV_INTERN
dberr_t
dict_create_add_tablespace_to_dictionary(
/*=====================================*/
	ulint		space,		/*!< in: tablespace id */
	const char*	name,		/*!< in: tablespace name */
	ulint		flags,		/*!< in: tablespace flags */
	const char*	path,		/*!< in: tablespace path */
	trx_t*		trx,		/*!< in: transaction */
	bool		commit);	/*!< in: if true then commit the
					transaction */

/** Add a single compression dictionary definition to the SYS_ZIP_DICT
InnoDB system table.
@return	error code or DB_SUCCESS */
UNIV_INTERN
dberr_t
dict_create_add_zip_dict(
	const char*	name,		/*!< in: dict name */
	ulint		name_len,	/*!< in: dict name length */
	const char*	data,		/*!< in: dict data */
	ulint		data_len,	/*!< in: dict data length */
	trx_t*		trx);		/*!< in/out: transaction */

/** Add a single compression dictionary reference to the SYS_ZIP_DICT_COLS
InnoDB system table.
@return	error code or DB_SUCCESS */
UNIV_INTERN
dberr_t
dict_create_add_zip_dict_reference(
	ulint		table_id,	/*!< in: table id */
	ulint		column_pos,	/*!< in: column position */
	ulint		dict_id,	/*!< in: dict id */
	trx_t*		trx);		/*!< in/out: transaction */

/** Get a single compression dictionary id for the given
(table id, column pos) pair.
@return	error code or DB_SUCCESS */
UNIV_INTERN
dberr_t
dict_create_get_zip_dict_id_by_reference(
	ulint	table_id,	/*!< in: table id */
	ulint	column_pos,	/*!< in: column position */
	ulint*	dict_id,	/*!< out: dict id */
	trx_t*	trx);		/*!< in/out: transaction */

/** Get compression dictionary id for the given name.
@return	error code or DB_SUCCESS */
UNIV_INTERN
dberr_t
dict_create_get_zip_dict_id_by_name(
	const char*	dict_name,	/*!< in: dict name */
	ulint		dict_name_len,	/*!< in: dict name length */
	ulint*		dict_id,	/*!< out: dict id */
	trx_t*		trx);		/*!< in/out: transaction */

/** Get compression dictionary info (name and data) for the given id.
Allocates memory for name and data on success.
Must be freed with mem_free().
@return	error code or DB_SUCCESS */
UNIV_INTERN
dberr_t
dict_create_get_zip_dict_info_by_id(
	ulint	dict_id,	/*!< in: dict id */
	char**	name,		/*!< out: dict name */
	ulint*	name_len,	/*!< out: dict name length */
	char**	data,		/*!< out: dict data */
	ulint*	data_len,	/*!< out: dict data length */
	trx_t*	trx);		/*!< in/out: transaction */

/** Remove a single compression dictionary from the data dictionary
tables in the database.
@return	error code or DB_SUCCESS */
UNIV_INTERN
dberr_t
dict_create_remove_zip_dict(
	const char*	name,		/*!< in: dict name */
	ulint		name_len,	/*!< in: dict name length */
	trx_t*		trx);		/*!< in/out: transaction */

/** Remove all compression dictionary references for the given table ID from
the data dictionary tables in the database.
@return	error code or DB_SUCCESS */
UNIV_INTERN
dberr_t
dict_create_remove_zip_dict_references_for_table(
	ulint	table_id,	/*!< in: table id */
	trx_t*	trx);		/*!< in/out: transaction */

/********************************************************************//**
Add a foreign key definition to the data dictionary tables.
@return	error code or DB_SUCCESS */
UNIV_INTERN
dberr_t
dict_create_add_foreign_to_dictionary(
/*==================================*/
	dict_table_t*		table,	/*!< in: table */
	const char*		name,	/*!< in: table name */
	const dict_foreign_t*	foreign,/*!< in: foreign key */
	trx_t*			trx)	/*!< in/out: dictionary transaction */
	MY_ATTRIBUTE((nonnull, warn_unused_result));

/* Table create node structure */
struct tab_node_t{
	que_common_t	common;	/*!< node type: QUE_NODE_TABLE_CREATE */
	dict_table_t*	table;	/*!< table to create, built as a memory data
				structure with dict_mem_... functions */
	ins_node_t*	tab_def; /* child node which does the insert of
				the table definition; the row to be inserted
				is built by the parent node  */
	ins_node_t*	col_def; /* child node which does the inserts of
				the column definitions; the row to be inserted
				is built by the parent node  */
	commit_node_t*	commit_node;
				/* child node which performs a commit after
				a successful table creation */
	/*----------------------*/
	/* Local storage for this graph node */
	ulint		state;	/*!< node execution state */
	ulint		col_no;	/*!< next column definition to insert */
	mem_heap_t*	heap;	/*!< memory heap used as auxiliary storage */
};

/* Table create node states */
#define	TABLE_BUILD_TABLE_DEF	1
#define	TABLE_BUILD_COL_DEF	2
#define	TABLE_COMMIT_WORK	3
#define	TABLE_ADD_TO_CACHE	4
#define	TABLE_COMPLETED		5

/* Index create node struct */

struct ind_node_t{
	que_common_t	common;	/*!< node type: QUE_NODE_INDEX_CREATE */
	dict_index_t*	index;	/*!< index to create, built as a memory data
				structure with dict_mem_... functions */
	ins_node_t*	ind_def; /* child node which does the insert of
				the index definition; the row to be inserted
				is built by the parent node  */
	ins_node_t*	field_def; /* child node which does the inserts of
				the field definitions; the row to be inserted
				is built by the parent node  */
	commit_node_t*	commit_node;
				/* child node which performs a commit after
				a successful index creation */
	/*----------------------*/
	/* Local storage for this graph node */
	ulint		state;	/*!< node execution state */
	ulint		page_no;/* root page number of the index */
	dict_table_t*	table;	/*!< table which owns the index */
	dtuple_t*	ind_row;/* index definition row built */
	ulint		field_no;/* next field definition to insert */
	mem_heap_t*	heap;	/*!< memory heap used as auxiliary storage */
};

/* Index create node states */
#define	INDEX_BUILD_INDEX_DEF	1
#define	INDEX_BUILD_FIELD_DEF	2
#define	INDEX_CREATE_INDEX_TREE	3
#define	INDEX_COMMIT_WORK	4
#define	INDEX_ADD_TO_CACHE	5

#ifndef UNIV_NONINL
#include "dict0crea.ic"
#endif

#endif
