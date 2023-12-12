// 한도관리를 위한 class
// Add 한근희
// 2000. 11. 6
class LIMIT_INFO
{
	private:
		unsigned int	nOperationID;        // 메시지를 구별하기 위한 unique key
		int				nSessionStartTime;   // 세션의 시작시간
		int				nSessionEndTime;     // 세션의 중간 정산 시간 
		int				nLimitValue;         // Access Request의 결과로 받은 금액(원)
		int				nNtopFreeTime;       // Access Request의 결과로 받은 금액(초)
		int				nUseValue;           // 사용자가 사용한 금액
		int				nUseTime;            // nTop사용자 사용시간
	public:
		unsigned int	get_nOperationID( void ) 		{ return nOperationID; 		}
		int				get_nSessionStartTime( void )	{ return nSessionStartTime; }
		int				get_nSessionEndTime( void )		{ return nSessionEndTime; 	}
		int				get_nLimitValue( void )			{ return nLimitValue; 		}
		int				get_nNtopFreeTime( void )		{ return nNtopFreeTime; 	}
		int				get_nUseValue( void )			{ return nUseValue; 		}
		int				get_nUseTime( void )			{ return nUseTime; 			}

		void			put_nOperationID( unsigned int value )	{ nOperationID=value; 		}
		void			put_nSessionStartTime( int value )		{ nSessionStartTime=value;	}
		void			put_nSessionEndTime( int value )        { nSessionEndTime=value; 	}
		void			put_nLimitValue( int value )			{ nLimitValue=value; 		}
		void			put_nNtopFreeTime( int value )			{ nNtopFreeTime=value; 		}
		void			put_nUseValue( int value )				{ nUseValue=value; 			}
		void			put_nUseTime( int value )				{ nUseTime=value; 			}

};
