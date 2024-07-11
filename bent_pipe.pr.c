/* Process model C form file: bent_pipe.pr.c */
/* Portions of this file copyright 1992-2003 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char bent_pipe_pr_c [] = "MIL_3_Tfile_Hdr_ 100A 30A op_runsim 7 40ED07C8 40ED07C8 1 opnet chunming 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 8f3 1                                                                                                                                                                                                                                                                                                                                                                                                          ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#define W_IN_STRM 0
#define E_IN_STRM 1
#define W_OUT_STRM 0
#define E_OUT_STRM 1
#define FROM_E (op_intrpt_strm () == E_IN_STRM) 
#define FROM_W (op_intrpt_strm () == W_IN_STRM) 
#define BEG_SIM (op_intrpt_type () == OPC_INTRPT_BEGSIM)


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
	} bent_pipe_state;

#define pr_state_ptr            		((bent_pipe_state*) (OP_SIM_CONTEXT_PTR->mod_state_ptr))

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#if defined (OPD_PARALLEL)
#  define FIN_PREAMBLE_DEC	bent_pipe_state *op_sv_ptr; OpT_Sim_Context * tcontext_ptr;
#  define FIN_PREAMBLE_CODE	\
		if (VosS_Mt_Perform_Lock) \
			VOS_THREAD_SPECIFIC_DATA_GET (VosI_Globals.simi_mt_context_data_key, tcontext_ptr, SimT_Context *); \
		else \
			tcontext_ptr = VosI_Globals.simi_sequential_context_ptr; \
		op_sv_ptr = ((bent_pipe_state *)(tcontext_ptr->mod_state_ptr));
#else
#  define FIN_PREAMBLE_DEC	bent_pipe_state *op_sv_ptr;
#  define FIN_PREAMBLE_CODE	op_sv_ptr = pr_state_ptr;
#endif


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
	void bent_pipe (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype bent_pipe_init (int * init_block_ptr);
	VosT_Address bent_pipe_alloc (VOS_THREAD_INDEX_ARG_COMMA VosT_Obtype, int);
	void bent_pipe_diag (OP_SIM_CONTEXT_ARG_OPT);
	void bent_pipe_terminate (OP_SIM_CONTEXT_ARG_OPT);
	void bent_pipe_svar (void *, const char *, void **);


	VosT_Fun_Status Vos_Define_Object (VosT_Obtype * obst_ptr, const char * name, unsigned int size, unsigned int init_obs, unsigned int inc_obs);
	VosT_Address Vos_Alloc_Object_MT (VOS_THREAD_INDEX_ARG_COMMA VosT_Obtype ob_hndl);
	VosT_Fun_Status Vos_Poolmem_Dealloc_MT (VOS_THREAD_INDEX_ARG_COMMA VosT_Address ob_ptr);
#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
bent_pipe (OP_SIM_CONTEXT_ARG_OPT)
	{

#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (bent_pipe ());
	if (1)
		{
		Objid w_cent_id;
		Objid w_sub_id;
		Objid w_tx_id;
		Objid e_tx_id;
		Objid e_sub_id;
		Objid e_cent_id;
		Objid e_ant_id;
		Objid w_ant_id;
		Packet*  pkptr;
		Objid my_id;
		double lat, lng, alt, x, y, z;


		FSM_ENTER ("bent_pipe")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "bent_pipe [init enter execs]")
				FSM_PROFILE_SECTION_IN (bent_pipe [init enter execs], state0_enter_exec)
				{
				/* obtain id of this module */
				my_id = op_id_self ();
				
				/* obtain the tx id, to get ant id */
				
				w_tx_id = op_topo_assoc (my_id, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_RATX, W_OUT_STRM); 
				e_tx_id = op_topo_assoc (my_id, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_RATX, E_OUT_STRM); 
				
				/* get ant id */
				
				w_ant_id = op_topo_assoc (w_tx_id, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_ANT, 0); 
				e_ant_id = op_topo_assoc (e_tx_id, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_ANT, 0); 
				
				/* obtain id of center nodes */
				
				w_sub_id = op_id_from_name (0, OPC_OBJTYPE_SUBNET, "west_net"); 
				e_sub_id = op_id_from_name (0, OPC_OBJTYPE_SUBNET, "east_net"); 
				w_cent_id = op_id_from_name (w_sub_id, OPC_OBJTYPE_NDFIX, "gnd_0"); 
				e_cent_id = op_id_from_name (e_sub_id, OPC_OBJTYPE_NDFIX, "gnd_0");
				 
				/* determine position of center nodes and point the antennas to those nodes */
				
				op_ima_node_pos_get (w_cent_id, &lat, &lng, &alt, &x, &y, &z); 
				op_ima_obj_attr_set (w_ant_id, "target altitude", alt); 
				op_ima_obj_attr_set (w_ant_id, "target longitude", lng); 
				op_ima_obj_attr_set (w_ant_id, "target latitude", lat);
				 
				op_ima_node_pos_get (e_cent_id, &lat, &lng, &alt, &x, &y, &z); 
				op_ima_obj_attr_set (e_ant_id, "target altitude", alt); 
				op_ima_obj_attr_set (e_ant_id, "target longitude", lng); 
				op_ima_obj_attr_set (e_ant_id, "target latitude", lat);
				
				}

				FSM_PROFILE_SECTION_OUT (bent_pipe [init enter execs], state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "bent_pipe [init exit execs]")
				FSM_PROFILE_SECTION_IN (bent_pipe [init exit execs], state0_exit_exec)
				{
				}
				FSM_PROFILE_SECTION_OUT (bent_pipe [init exit execs], state0_exit_exec)


			/** state (init) transition processing **/
			FSM_TRANSIT_ONLY ((BEG_SIM), 1, state1_enter_exec, ;, init, "BEG_SIM", "", "init", "idle")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "idle", state1_enter_exec, "bent_pipe [idle enter execs]")
				FSM_PROFILE_SECTION_IN (bent_pipe [idle enter execs], state1_enter_exec)
				{
				}

				FSM_PROFILE_SECTION_OUT (bent_pipe [idle enter execs], state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"bent_pipe")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "idle", "bent_pipe [idle exit execs]")
				FSM_PROFILE_SECTION_IN (bent_pipe [idle exit execs], state1_exit_exec)
				{
				pkptr = op_pk_get(op_intrpt_strm());
				}
				FSM_PROFILE_SECTION_OUT (bent_pipe [idle exit execs], state1_exit_exec)


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN (bent_pipe [idle trans conditions], state1_trans_conds)
			FSM_INIT_COND (FROM_W)
			FSM_TEST_COND (FROM_E)
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (bent_pipe [idle trans conditions], state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "FROM_W", "", "idle", "from W")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "FROM_E", "", "idle", "from E")
				}
				/*---------------------------------------------------------*/



			/** state (from W) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "from W", state2_enter_exec, "bent_pipe [from W enter execs]")
				FSM_PROFILE_SECTION_IN (bent_pipe [from W enter execs], state2_enter_exec)
				{
				op_pk_send (pkptr, E_OUT_STRM);
				
				}

				FSM_PROFILE_SECTION_OUT (bent_pipe [from W enter execs], state2_enter_exec)

			/** state (from W) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "from W", "bent_pipe [from W exit execs]")
				FSM_PROFILE_SECTION_IN (bent_pipe [from W exit execs], state2_exit_exec)
				{
				}
				FSM_PROFILE_SECTION_OUT (bent_pipe [from W exit execs], state2_exit_exec)


			/** state (from W) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "from W", "idle")
				/*---------------------------------------------------------*/



			/** state (from E) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "from E", state3_enter_exec, "bent_pipe [from E enter execs]")
				FSM_PROFILE_SECTION_IN (bent_pipe [from E enter execs], state3_enter_exec)
				{
				op_pk_send (pkptr, W_OUT_STRM);
				
				}

				FSM_PROFILE_SECTION_OUT (bent_pipe [from E enter execs], state3_enter_exec)

			/** state (from E) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "from E", "bent_pipe [from E exit execs]")
				FSM_PROFILE_SECTION_IN (bent_pipe [from E exit execs], state3_exit_exec)
				{
				}
				FSM_PROFILE_SECTION_OUT (bent_pipe [from E exit execs], state3_exit_exec)


			/** state (from E) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "from E", "idle")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"bent_pipe")
		}
	}




void
bent_pipe_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
bent_pipe_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

#if !defined (VOSD_NO_FIN)
	int _op_block_origin = __LINE__;
#endif

	FIN_MT (bent_pipe_terminate ())

	Vos_Poolmem_Dealloc_MT (OP_SIM_CONTEXT_THREAD_INDEX_COMMA pr_state_ptr);

	FOUT
	}


#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
bent_pipe_init (int * init_block_ptr)
	{

#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (bent_pipe_init (init_block_ptr))

	Vos_Define_Object (&obtype, "proc state vars (bent_pipe)",
		sizeof (bent_pipe_state), 0, 20);
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
bent_pipe_alloc (VOS_THREAD_INDEX_ARG_COMMA VosT_Obtype obtype, int init_block)
	{

#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	bent_pipe_state * ptr;
	FIN_MT (bent_pipe_alloc (obtype))

	ptr = (bent_pipe_state *)Vos_Alloc_Object_MT (VOS_THREAD_INDEX_COMMA obtype);
	if (ptr != OPC_NIL)
		ptr->_op_current_block = init_block;
	FRET ((VosT_Address)ptr)
	}



void
bent_pipe_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{

	FIN_MT (bent_pipe_svar (gen_ptr, var_name, var_p_ptr))

	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

