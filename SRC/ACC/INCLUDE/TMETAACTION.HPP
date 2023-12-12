/*
 * $Log: TMetaAction.hpp $
 * Revision 0.9.0  2001/04/13 16:05:00  dongcheol
 * table name : Last 작업정보  table id : t_meta_action
 * Revision 0.9.1  ...
 *
 */


#include "CSQLException.hpp"
#include <vector>

#ifndef _TMETAACTION_HPP_
#define _TMETAACTION_HPP_

namespace acc {

#define  LEN_MAPPING_NAME      8 
#define  LEN_VER               5 
#define  LEN_SEQ_NO            4 
#define  LEN_IN_FIELD_NAME     30
#define  LEN_IN_FIELD_LENGTH   4 
#define  LEN_IN_FIELD_OFFSET   4 
#define  LEN_OUT_FIELD_NAME    30
#define  LEN_OUT_FIELD_LENGTH  4 
#define  LEN_OUT_FIELD_OFFSET  4 
#define  LEN_ACTION_CODE       2 
#define  LEN_SUB_ACTION_CODE   2 
#define  LEN_DATA_TYPE         2 


class TMetaAction {
public:
    explicit  TMetaAction();
    virtual  ~TMetaAction();

    void      setMappingName   (const string&);
    void      setVer           (const string&);
    void      setSeqNo         (const string&);
    void      setInFieldName   (const string&);
    void      setInFieldLength (const string&);
    void      setInFieldOffset (const string&);
    void      setOutFieldName  (const string&);
    void      setOutFieldLength(const string&);
    void      setOutFieldOffset(const string&);
    void      setActionCode    (const string&);
    void      setSubActionCode (const string&);
    void      setDataType      (const string&);

    const string&  getMappingName   () const;
    const string&  getVer           () const;
    const string&  getSeqNo         () const;
    const string&  getInFieldName   () const;
    const string&  getInFieldLength () const;
    const string&  getInFieldOffset () const;
    const string&  getOutFieldName  () const;
    const string&  getOutFieldLength() const;
    const string&  getOutFieldOffset() const;
    const string&  getActionCode    () const;
    const string&  getSubActionCode () const;
    const string&  getDataType      () const;

    void     clear();
    void     select(vector<TMetaAction>&) throw (CSQLException, bad_exception);

protected:
    void     saveTextOfSQL();

private:
    string  MAPPING_NAME_    ;
    string  VER_             ;
    string  SEQ_NO_          ;
    string  IN_FIELD_NAME_   ;
    string  IN_FIELD_LENGTH_ ;
    string  IN_FIELD_OFFSET_ ;
    string  OUT_FIELD_NAME_  ;
    string  OUT_FIELD_LENGTH_;
    string  OUT_FIELD_OFFSET_;
    string  ACTION_CODE_     ;
    string  SUB_ACTION_CODE_ ;
    string  DATA_TYPE_       ;
};          
            
}; // end of namespace acc
#endif   // _TMETAACTION_HPP_
