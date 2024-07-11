/* Process model C form file: tdma3.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char tdma3_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 668F8809 668F8809 1 ray-laptop 28918 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                         ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#include <math.h>

/* Constant Definitions */
#define RX_IN_STRM		(1)
#define SRC_IN_STRM		(0)
#define TX_OUT_STRM		(1)
#define SINK_OUT_STRM	(0)

#define EPSILON  		(1e-10)  /* rounding error factor */
#define TDMA_COMPLETE	(-10)


/* Transition Condition Macros */ 
#define FROM_RX			(op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm () == RX_IN_STRM)
#define FROM_SRC 		(op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm () == SRC_IN_STRM) 
#define TRANSMITTING	(op_stat_local_read (0) == 1.0) 
#define SLOT 			(op_intrpt_type() == OPC_INTRPT_SELF)
#define END  			(op_intrpt_type() == OPC_INTRPT_STAT)
#define DATA_ENQ 		(!(op_subq_empty (0)))

#define	SELF_INTRPT_SCHLD	(intrpt_flag == 1)

/* Global Variables */
int		tdma_pk_sent;
int		tdma_pk_rcvd;
int		tdma_bits_sent;
int		tdma_bits_rcvd;
int		tdma_setup;
int		tdma_id;
int		num_slots;

/* End of Header Block */

#if !defined (VOSD_NO_FIN)
#undef	BIN
#undef	BOUT
#define	BIN		FIN_LOCAL_FIELD(_op_last_line_passed) = __LINE__ - _op_block_origin;
#define	BOUT	BIN
#define	BINIT	FIN_LOCAL_FIELD(_op_last_line_passed) = 0; _op_block_origin = __LINE__;
#else
#define	BINIT
#endif /* #if !defined (VOSD_NO_FIN) */



/* State variable definitions */
typedef struct
	{
	/* Internal state tracking for FSM */
	FSM_SYS_STATE
	/* State Variables */
	int	                    		my_offset                                       ;
	double	                 		slot_length                                     ;
	double	                 		tx_data_rate                                    ;
	int	                    		intrpt_flag                                     ;
	int	                    		num_pk_sent                                     ;
	int	                    		num_pk_rcvd                                     ;
	int	                    		num_bits_sent                                   ;
	int	                    		num_bits_rcvd                                   ;
	Stathandle	             		num_pk_sent_stat                                ;
	Stathandle	             		global_pk_sent_stat                             ;
	Stathandle	             		num_pk_rcvd_stat                                ;
	Stathandle	             		global_pk_rcvd_stat                             ;
	Objid	                  		my_node_id                                      ;
	Objid	                  		my_id                                           ;
	Stathandle	             		num_bits_sent_stat                              ;
	Stathandle	             		global_bits_sent_stat                           ;
	Stathandle	             		num_bits_rcvd_stat                              ;
	Stathandle	             		global_bits_rcvd_stat                           ;
	Stathandle	             		bits_sec_rcvd_stat                              ;
	Stathandle	             		bits_sec_sent_stat                              ;
	Stathandle	             		pk_sec_rcvd_stat                                ;
	Stathandle	             		pk_sec_sent_stat                                ;
	Stathandle	             		global_bits_sec_rcvd_stat                       ;
	Stathandle	             		global_bits_sec_sent_stat                       ;
	Stathandle	             		global_pk_sec_rcvd_stat                         ;
	Stathandle	             		global_pk_sec_sent_stat                         ;
	int	                    		num_slots                                       ;
	} tdma3_state;

#define my_offset               		op_sv_ptr->my_offset
#define slot_length             		op_sv_ptr->slot_length
#define tx_data_rate            		op_sv_ptr->tx_data_rate
#define intrpt_flag             		op_sv_ptr->intrpt_flag
#define num_pk_sent             		op_sv_ptr->num_pk_sent
#define num_pk_rcvd             		op_sv_ptr->num_pk_rcvd
#define num_bits_sent           		op_sv_ptr->num_bits_sent
#define num_bits_rcvd           		op_sv_ptr->num_bits_rcvd
#define num_pk_sent_stat        		op_sv_ptr->num_pk_sent_stat
#define global_pk_sent_stat     		op_sv_ptr->global_pk_sent_stat
#define num_pk_rcvd_stat        		op_sv_ptr->num_pk_rcvd_stat
#define global_pk_rcvd_stat     		op_sv_ptr->global_pk_rcvd_stat
#define my_node_id              		op_sv_ptr->my_node_id
#define my_id                   		op_sv_ptr->my_id
#define num_bits_sent_stat      		op_sv_ptr->num_bits_sent_stat
#define global_bits_sent_stat   		op_sv_ptr->global_bits_sent_stat
#define num_bits_rcvd_stat      		op_sv_ptr->num_bits_rcvd_stat
#define global_bits_rcvd_stat   		op_sv_ptr->global_bits_rcvd_stat
#define bits_sec_rcvd_stat      		op_sv_ptr->bits_sec_rcvd_stat
#define bits_sec_sent_stat      		op_sv_ptr->bits_sec_sent_stat
#define pk_sec_rcvd_stat        		op_sv_ptr->pk_sec_rcvd_stat
#define pk_sec_sent_stat        		op_sv_ptr->pk_sec_sent_stat
#define global_bits_sec_rcvd_stat		op_sv_ptr->global_bits_sec_rcvd_stat
#define global_bits_sec_sent_stat		op_sv_ptr->global_bits_sec_sent_stat
#define global_pk_sec_rcvd_stat 		op_sv_ptr->global_pk_sec_rcvd_stat
#define global_pk_sec_sent_stat 		op_sv_ptr->global_pk_sec_sent_stat
#define num_slots               		op_sv_ptr->num_slots

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	tdma3_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((tdma3_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* No Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ };
#endif

/* Undefine optional tracing in FIN/FOUT/FRET */
/* The FSM has its own tracing code and the other */
/* functions should not have any tracing.		  */
#undef FIN_TRACING
#define FIN_TRACING

#undef FOUTRET_TRACING
#define FOUTRET_TRACING

#if defined (__cplusplus)
extern "C" {
#endif
	void tdma3 (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_tdma3_init (int * init_block_ptr);
	void _op_tdma3_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_tdma3_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_tdma3_alloc (VosT_Obtype, int);
	void _op_tdma3_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
tdma3 (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (tdma3 ());

		{
		/* Temporary Variables */
		Objid tx_id,comp_id,tx_ch_id;
		Packet* pkptr;
		int pk_len;
		double current_time;
		int used_slots;
		int current_offset;
		double time_left_in_slot;
		double pk_time;
		double next_offset;
		double my_next_slot_time;
		/* End of Temporary Variables */


		FSM_ENTER ("tdma3")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_UNFORCED_NOLABEL (0, "init", "tdma3 [init enter execs]")
				FSM_PROFILE_SECTION_IN ("tdma3 [init enter execs]", state0_enter_exec)
				{
				/* Get the slot length for all nodes */
				op_ima_sim_attr_get (OPC_IMA_DOUBLE, "Slot Length", &slot_length);
				
				my_id = op_id_self();
				my_node_id = op_topo_parent (my_id);
				
				/* Initialize tdma offsets if not done previously */
				if (tdma_setup != TDMA_COMPLETE)
				{
					num_slots = 0;
					tdma_setup = TDMA_COMPLETE;
				}
				
				/* Calculate the offset for this node */
				num_slots++;
				my_offset = num_slots-1;	
				
				if (op_prg_odb_ltrace_active ("tdma"))
				{
					printf ("Node Objid = %d\n", my_node_id);
					printf ("Node Offset = %d\n", my_offset);
					printf ("\n");
				}
					
				/*Determine the number of various types of nodes in the network */ 
				/* Determine the data rate for the transmitter */
				tx_id =  op_topo_assoc (my_id, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_RATX, 0); 
				comp_id = op_topo_child (tx_id, OPC_OBJTYPE_COMP, 0);
				tx_ch_id = op_topo_child (comp_id, OPC_OBJTYPE_RATXCH, 0);
				op_ima_obj_attr_get (tx_ch_id, "data rate", &tx_data_rate); 
				
				/* Initialize statistic calculation variables */
				
				/*
				tdma_pk_sent = 0;
				tdma_pk_rcvd = 0;
				tdma_bits_sent = 0;
				tdma_bits_rcvd = 0;
				num_pk_sent = 0;
				num_pk_rcvd = 0;
				num_bits_sent = 0;
				num_bits_rcvd = 0;
				*/
				
				/* Register Statistics */
				num_pk_sent_stat = op_stat_reg ("TDMA.Load (packets)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				global_pk_sent_stat = op_stat_reg ("TDMA.TDMA Load (packets)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				num_pk_rcvd_stat = op_stat_reg ("TDMA.Traffic Received (packets)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				global_pk_rcvd_stat = op_stat_reg ("TDMA.TDMA Traffic Received (packets)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				num_bits_sent_stat = op_stat_reg ("TDMA.Load (bits)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				global_bits_sent_stat = op_stat_reg ("TDMA.TDMA Load (bits)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				num_bits_rcvd_stat = op_stat_reg ("TDMA.Traffic Received (bits)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				global_bits_rcvd_stat = op_stat_reg ("TDMA.TDMA Traffic Received (bits)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				bits_sec_rcvd_stat = op_stat_reg ("TDMA.Traffic Received (bits/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				bits_sec_sent_stat = op_stat_reg ("TDMA.Load (bits/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				pk_sec_rcvd_stat = op_stat_reg ("TDMA.Traffic Received (packets/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				pk_sec_sent_stat = op_stat_reg ("TDMA.Load (packets/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				
				global_bits_sec_rcvd_stat = op_stat_reg ("TDMA.TDMA Traffic Received (bits/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				global_pk_sec_rcvd_stat = op_stat_reg ("TDMA.TDMA Traffic Received (bits/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				global_pk_sec_sent_stat = op_stat_reg ("TDMA.TDMA Load (packets/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				global_bits_sec_sent_stat = op_stat_reg ("TDMA.TDMA Load (packets/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				/* Schedule interupt to complete initialization in the exit execs */
				op_intrpt_schedule_self (op_sim_time (), 0);
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"tdma3")


			/** state (init) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "init", "tdma3 [init exit execs]")
				FSM_PROFILE_SECTION_IN ("tdma3 [init exit execs]", state0_exit_exec)
				{
				
				}
				FSM_PROFILE_SECTION_OUT (state0_exit_exec)


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "idle", "tr_12", "tdma3 [init -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "idle", state1_enter_exec, "tdma3 [idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"tdma3")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "idle", "tdma3 [idle exit execs]")


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("tdma3 [idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (FROM_RX)
			FSM_TEST_COND (FROM_SRC)
			FSM_TEST_COND ((END || SLOT) && DATA_ENQ)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "FROM_RX", "", "idle", "fr_rx", "tr_13", "tdma3 [idle -> fr_rx : FROM_RX / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "FROM_SRC", "", "idle", "fr_src", "tr_15", "tdma3 [idle -> fr_src : FROM_SRC / ]")
				FSM_CASE_TRANSIT (2, 4, state4_enter_exec, ;, "(END || SLOT) && DATA_ENQ", "", "idle", "tx", "tr_19", "tdma3 [idle -> tx : (END || SLOT) && DATA_ENQ / ]")
				FSM_CASE_TRANSIT (3, 1, state1_enter_exec, ;, "default", "", "idle", "idle", "tr_10", "tdma3 [idle -> idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (fr_rx) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "fr_rx", state2_enter_exec, "tdma3 [fr_rx enter execs]")
				FSM_PROFILE_SECTION_IN ("tdma3 [fr_rx enter execs]", state2_enter_exec)
				{
				pkptr =  op_pk_get (RX_IN_STRM);
				
				pk_len = (double) op_pk_total_size_get (pkptr);
				
				/** Record Statistics **/
				/** The bits/sec or packets/sec statistics are recorded in		**/
				/** bits and packets, and then the OPNET statistic "capture		**/
				/** mode" is used to obtain a bucketized sum over time.			**/
				/** Record extra 0.0 data-points to enable proper computation	**/
				/** of the "sum/time" based statistics.							**/
				
				op_stat_write (num_pk_rcvd_stat, 1.0);
				op_stat_write (pk_sec_rcvd_stat, 1.0);
				op_stat_write (pk_sec_rcvd_stat, 0.0);
				
				op_stat_write (global_pk_rcvd_stat, 1.0);
				op_stat_write (global_pk_sec_rcvd_stat, 1.0);
				op_stat_write (global_pk_sec_rcvd_stat, 0.0);
				
				op_stat_write (num_bits_rcvd_stat, pk_len);
				op_stat_write (bits_sec_rcvd_stat, pk_len);
				op_stat_write (bits_sec_rcvd_stat, 0.0);
				
				op_stat_write (global_bits_rcvd_stat, pk_len);
				op_stat_write (global_bits_sec_rcvd_stat, pk_len);
				op_stat_write (global_bits_sec_rcvd_stat, 0.0);
				
				op_pk_send (pkptr, SINK_OUT_STRM);
				
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (fr_rx) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "fr_rx", "tdma3 [fr_rx exit execs]")


			/** state (fr_rx) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "fr_rx", "idle", "tr_14", "tdma3 [fr_rx -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (fr_src) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "fr_src", state3_enter_exec, "tdma3 [fr_src enter execs]")
				FSM_PROFILE_SECTION_IN ("tdma3 [fr_src enter execs]", state3_enter_exec)
				{
				pkptr =  op_pk_get (SRC_IN_STRM);
				op_subq_pk_insert (0, pkptr, OPC_QPOS_TAIL); 
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (fr_src) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "fr_src", "tdma3 [fr_src exit execs]")


			/** state (fr_src) transition processing **/
			FSM_PROFILE_SECTION_IN ("tdma3 [fr_src trans conditions]", state3_trans_conds)
			FSM_INIT_COND (TRANSMITTING)
			FSM_TEST_COND (!TRANSMITTING)
			FSM_TEST_LOGIC ("fr_src")
			FSM_PROFILE_SECTION_OUT (state3_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "TRANSMITTING", "", "fr_src", "idle", "tr_16", "tdma3 [fr_src -> idle : TRANSMITTING / ]")
				FSM_CASE_TRANSIT (1, 4, state4_enter_exec, ;, "!TRANSMITTING", "", "fr_src", "tx", "tr_20", "tdma3 [fr_src -> tx : !TRANSMITTING / ]")
				}
				/*---------------------------------------------------------*/



			/** state (tx) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "tx", state4_enter_exec, "tdma3 [tx enter execs]")
				FSM_PROFILE_SECTION_IN ("tdma3 [tx enter execs]", state4_enter_exec)
				{
				current_time = op_sim_time();
				
				/* Determine if currently my slot. */
				/* EPSILON accounts for rounding error */
				used_slots = (int) floor ((current_time / slot_length) + EPSILON);
				
				current_offset = used_slots % num_slots;
				
				/* See if there is time left in the current slot to transmit */
				/* the packet at the top of the queue */
				time_left_in_slot = ((used_slots + 1)*slot_length) - current_time;
				
				pk_len = (double) op_pk_total_size_get (op_subq_pk_access (0, OPC_QPOS_HEAD)); 
				pk_time = (double) pk_len / tx_data_rate;
				
				/* If this is my slot and I have enough time to transmit the */
				/* entire packet then transmit. Otherwise set a self intrpt  */
				/* for the beginning of my next slot.                        */
				if ((current_offset == my_offset) && (pk_time < time_left_in_slot))
					{
				
					/* dequeue the packet and send it */
					pkptr = op_subq_pk_remove (0, OPC_QPOS_HEAD);
				
					/* reset the flag to schedule a self interrupt */
					/* for packets arriving subsequent to this one */
					intrpt_flag = 0;
				
					if (op_prg_odb_ltrace_active ("tdma"))
					{
						printf ("TDMA Node %d is transmitting at time: %f\n", my_node_id, op_sim_time ());
						printf ("\n");
					}
					
					pk_len = (double) op_pk_total_size_get (pkptr);
				
					/** Record Statistics **/
					/** The bits/sec or packets/sec statistics are recorded in		**/
					/** bits and packets, and then the OPNET statistic "capture		**/
					/** mode" is used to obtain a bucketized sum over time.			**/
					/** Record extra 0.0 data-points to enable proper computation	**/
					/** of the "sum/time" based statistics.							**/
				
					op_stat_write (num_pk_sent_stat, 1.0);
					op_stat_write (pk_sec_sent_stat, 1.0);
					op_stat_write (pk_sec_sent_stat, 0.0);
				
					op_stat_write (global_pk_sent_stat, 1.0);
					op_stat_write (global_pk_sec_sent_stat, 1.0);
					op_stat_write (global_pk_sec_sent_stat, 0.0);
				
					op_stat_write (num_bits_sent_stat, pk_len);
					op_stat_write (bits_sec_sent_stat, pk_len);
					op_stat_write (bits_sec_sent_stat, 0.0);
				
					op_stat_write (global_bits_sent_stat, pk_len);
					op_stat_write (global_bits_sec_sent_stat, pk_len);
					op_stat_write (global_bits_sec_sent_stat, 0.0);
				
					op_pk_send (pkptr, TX_OUT_STRM);
				
					} /* End if */
				else
					{
				
					next_offset = my_offset - current_offset;
				
					if (next_offset <= 0)
						{
						next_offset += num_slots;
						}
				
				
					my_next_slot_time = (double) (used_slots + next_offset) * slot_length;
				
					/* schedule a self interrupt only if one has not */
					/* been already scheduled for the next slot time */
					if (!SELF_INTRPT_SCHLD)
						{
						op_intrpt_schedule_self (my_next_slot_time, 0); 
						intrpt_flag = 1;
						}
					} /* End else */
				
				
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (tx) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "tx", "tdma3 [tx exit execs]")


			/** state (tx) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "tx", "idle", "tr_18", "tdma3 [tx -> idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"tdma3")
		}
	}




void
_op_tdma3_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
#if defined (OPD_ALLOW_ODB)
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = __LINE__+1;
#endif

	FIN_MT (_op_tdma3_diag ())

	if (1)
		{
		/* Temporary Variables */
		Objid tx_id,comp_id,tx_ch_id;
		Packet* pkptr;
		int pk_len;
		double current_time;
		int used_slots;
		int current_offset;
		double time_left_in_slot;
		double pk_time;
		double next_offset;
		double my_next_slot_time;
		/* End of Temporary Variables */

		/* Diagnostic Block */

		BINIT
		{
		printf ("Object ID = %d Current Sim Time = %g\n", my_id, op_sim_time ());
		printf ("My TDMA Offset = %d\n", my_offset);
		printf ("Number of TDMA Slots = %d\n", num_slots);
		printf ("Number of Packets Received = %d\n", num_pk_rcvd);
		printf ("Number of Bits Received = %d\n", num_bits_rcvd);
		printf ("Number of Packets Sent = %d\n", num_pk_sent);
		printf ("Number of Bits Sent = %d\n", num_bits_sent);
		
		}

		/* End of Diagnostic Block */

		}

	FOUT
#endif /* OPD_ALLOW_ODB */
	}




void
_op_tdma3_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_tdma3_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_tdma3_svar function. */
#undef my_offset
#undef slot_length
#undef tx_data_rate
#undef intrpt_flag
#undef num_pk_sent
#undef num_pk_rcvd
#undef num_bits_sent
#undef num_bits_rcvd
#undef num_pk_sent_stat
#undef global_pk_sent_stat
#undef num_pk_rcvd_stat
#undef global_pk_rcvd_stat
#undef my_node_id
#undef my_id
#undef num_bits_sent_stat
#undef global_bits_sent_stat
#undef num_bits_rcvd_stat
#undef global_bits_rcvd_stat
#undef bits_sec_rcvd_stat
#undef bits_sec_sent_stat
#undef pk_sec_rcvd_stat
#undef pk_sec_sent_stat
#undef global_bits_sec_rcvd_stat
#undef global_bits_sec_sent_stat
#undef global_pk_sec_rcvd_stat
#undef global_pk_sec_sent_stat
#undef num_slots

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_tdma3_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_tdma3_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (tdma3)",
		sizeof (tdma3_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_tdma3_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	tdma3_state * ptr;
	FIN_MT (_op_tdma3_alloc (obtype))

	ptr = (tdma3_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "tdma3 [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_tdma3_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	tdma3_state		*prs_ptr;

	FIN_MT (_op_tdma3_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (tdma3_state *)gen_ptr;

	if (strcmp ("my_offset" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_offset);
		FOUT
		}
	if (strcmp ("slot_length" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->slot_length);
		FOUT
		}
	if (strcmp ("tx_data_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->tx_data_rate);
		FOUT
		}
	if (strcmp ("intrpt_flag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->intrpt_flag);
		FOUT
		}
	if (strcmp ("num_pk_sent" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->num_pk_sent);
		FOUT
		}
	if (strcmp ("num_pk_rcvd" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->num_pk_rcvd);
		FOUT
		}
	if (strcmp ("num_bits_sent" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->num_bits_sent);
		FOUT
		}
	if (strcmp ("num_bits_rcvd" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->num_bits_rcvd);
		FOUT
		}
	if (strcmp ("num_pk_sent_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->num_pk_sent_stat);
		FOUT
		}
	if (strcmp ("global_pk_sent_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->global_pk_sent_stat);
		FOUT
		}
	if (strcmp ("num_pk_rcvd_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->num_pk_rcvd_stat);
		FOUT
		}
	if (strcmp ("global_pk_rcvd_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->global_pk_rcvd_stat);
		FOUT
		}
	if (strcmp ("my_node_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_node_id);
		FOUT
		}
	if (strcmp ("my_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_id);
		FOUT
		}
	if (strcmp ("num_bits_sent_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->num_bits_sent_stat);
		FOUT
		}
	if (strcmp ("global_bits_sent_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->global_bits_sent_stat);
		FOUT
		}
	if (strcmp ("num_bits_rcvd_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->num_bits_rcvd_stat);
		FOUT
		}
	if (strcmp ("global_bits_rcvd_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->global_bits_rcvd_stat);
		FOUT
		}
	if (strcmp ("bits_sec_rcvd_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bits_sec_rcvd_stat);
		FOUT
		}
	if (strcmp ("bits_sec_sent_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bits_sec_sent_stat);
		FOUT
		}
	if (strcmp ("pk_sec_rcvd_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pk_sec_rcvd_stat);
		FOUT
		}
	if (strcmp ("pk_sec_sent_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pk_sec_sent_stat);
		FOUT
		}
	if (strcmp ("global_bits_sec_rcvd_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->global_bits_sec_rcvd_stat);
		FOUT
		}
	if (strcmp ("global_bits_sec_sent_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->global_bits_sec_sent_stat);
		FOUT
		}
	if (strcmp ("global_pk_sec_rcvd_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->global_pk_sec_rcvd_stat);
		FOUT
		}
	if (strcmp ("global_pk_sec_sent_stat" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->global_pk_sec_sent_stat);
		FOUT
		}
	if (strcmp ("num_slots" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->num_slots);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

