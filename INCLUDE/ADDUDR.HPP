// �ѵ������� ���� class
// Add �ѱ���
// 2000. 11. 6
class LIMIT_INFO
{
	private:
		unsigned int	nOperationID;        // �޽����� �����ϱ� ���� unique key
		int				nSessionStartTime;   // ������ ���۽ð�
		int				nSessionEndTime;     // ������ �߰� ���� �ð� 
		int				nLimitValue;         // Access Request�� ����� ���� �ݾ�(��)
		int				nNtopFreeTime;       // Access Request�� ����� ���� �ݾ�(��)
		int				nUseValue;           // ����ڰ� ����� �ݾ�
		int				nUseTime;            // nTop����� ���ð�
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