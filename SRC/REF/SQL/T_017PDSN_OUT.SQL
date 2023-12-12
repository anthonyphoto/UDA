------------------------------------------------------------------
-- T_017PDSN_OUT : 017PDSN OUTPUT Table                         --
------------------------------------------------------------------
CREATE TABLE T_017PDSN_OUT
(SEQ_NO                 NUMBER(4,0)         NOT NULL,      /*Field Offset            */
 PDSN_CODE              NUMBER(4,0)         NOT NULL,      /* Action Code            */
 VENDOR_CODE            NUMBER(4,0)         DEFAULT 0,     /* Sub Action Code        */
 CONSTRAINT T_017PDSN_OUT_PK PRIMARY KEY
 	(SEQ_NO)
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