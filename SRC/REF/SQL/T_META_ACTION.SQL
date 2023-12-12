------------------------------------------------------------------
-- T_META_ACTION : META ACTION Table                              --
------------------------------------------------------------------
CREATE TABLE T_META_ACTION
(MAPPING_NAME               CHAR(4)             NOT NULL,      /*Output Name             */
 VER                        CHAR(5)             NOT NULL,      /*Version                 */
 SEQ_NO                     NUMBER(4,0)         NOT NULL,      /*Field Offset            */
 IN_FIELD_NAME              VARCHAR2(30)        DEFAULT ' ',   /*Field Name              */
 IN_FIELD_LENGTH            NUMBER(4,0)         DEFAULT 0,     /*Field Length            */
 IN_FIELD_OFFSET    	    NUMBER(4,0)         DEFAULT 0,     /*Field Offset            */
 OUT_FIELD_NAME             VARCHAR2(30)        DEFAULT ' ',   /*Field Name              */
 OUT_FIELD_LENGTH           NUMBER(4,0)         DEFAULT 0,     /*Field Length            */
 OUT_FIELD_OFFSET    	    NUMBER(4,0)         DEFAULT 0,     /*Field Offset            */
 ACTION_CODE                CHAR(2)             DEFAULT ' ',   /* Action Code            */
 SUB_ACTION_CODE            CHAR(2)             DEFAULT ' ',   /* Sub Action Code        */
 DATA_TYPE                  CHAR(2)             DEFAULT ' ',   /* Data Type              */
 CONSTRAINT T_META_ACTION_PK PRIMARY KEY
 	(MAPPING_NAME,  VER,  SEQ_NO)
         USING INDEX  TABLESPACE ICBS_TEST_IDX       /* INDEX명과 CONSTRAINT명이 같음 */
         STORAGE (INITIAL 33K
                  NEXT 33K
                  PCTINCREASE 0
                  MINEXTENTS 1
                  MAXEXTENTS 50))
 PCTFREE 5
 PCTUSED 70
 STORAGE (INITIAL     240K
          NEXT        240K
          PCTINCREASE 0
          MINEXTENTS  1
          MAXEXTENTS  50)
 TABLESPACE ICBS_TEST_DAT;