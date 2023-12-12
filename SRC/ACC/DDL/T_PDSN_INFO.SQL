
--drop table T_PDSN_INFO;

--------------------------------------------------------------
--	Description 			: 	PDSN INFO					--
--	Table Name				:	T_PDSN_INFO					--
--	Data Tablespace Name	:	UDA1_TABS_CONF_DAT			--
--	Index Tablespace Name	:	UDA1_TABS_CONF_IDX			--
--------------------------------------------------------------
create table T_PDSN_INFO
	(	
		PDSN_IP_ADDR	varchar2(15)	not null,
		PDSN_LOC 		varchar2(32)	not null,
		constraint T_PDSN_INFO_PK primary key
			(PDSN_IP_ADDR)
			using index tablespace UDA1_TABS_CONF_IDX
				storage
				(	initial 	1500
					next 		1500
					pctincrease 0
					minextents 	1
					maxextents 	10
				)
	)
	pctfree 5
	pctused 70
	storage 
	(	initial     10K
		next        10K
		pctincrease 0
		minextents  1
		maxextents  10
	)
	tablespace  UDA1_TABS_CONF_DAT;
