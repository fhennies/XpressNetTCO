#define MCP
#define XPRESSNET

#ifdef XPRESSNET
	#ifdef MCP
		#define PINSATMEL {2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15, 16, 17}
	#else
		#define PINSATMEL {2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19}
	#endif
#endif

// Adressen shifted by 1 for z21PG (6 sent here is 7 IRL)
#define SHIFTADDR	1

#define INATMEL			{3, 4, 5, 6, 7, 8, 10, 11, 12,  2, 14, 15, 16, 17}	//all inputs
                                    //D2-D8 (2-8); D10-D13(10-13); A0-A5 (14-19) (A4, A5 not if MCP)
#define INATMELADD		{7, 7, 9, 9, 4, 4,  3,  3,  6,  6,  5,  5,  8,  8}	//Addr 1 for inputs
#define INATMELPOS		{1, 0, 0, 1, 1, 0,  1,  0,  0,  1,  1,  0,  1,  0}	//Pos each Addr 1
#define INATMELADD2	    {0, 0, 0, 0, 2, 2,  2,  2,  0,  0,  0,  0,  0,  0}	//Addr 2 for inputs, "0" if not in use
#define INATMELPOS2	    {0, 0, 0, 0, 1, 1,  0,  0,  0,  0,  0,  0,  0,  0}	//Pos each Addr 2, "0" if not in use

#define OUTATMEL		{}			//all outputs
#define OUTATMELADD	    {} 			//Addr for outputs
#define OUTATMELPOS	    {}	    	//Pos each Addr

#define OUTMCP			{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}	//all outputs 
                                    //0-7 port A; 8-15 port B
#define OUTMCPADD		{6, 6, 5, 5, 7, 7, 8, 8, 9, 9,  4,  4,  3,  3,  2,  2}	//Addr for outputs
#define OUTMCPPOS		{0, 1, 1, 0, 1, 0, 1, 0, 1, 0,  0,  1,  0,  1,  0,  1}	//Pos each Addr