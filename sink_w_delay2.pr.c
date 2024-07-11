/* Process model C form file: sink_w_delay2.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char sink_w_delay2_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 668F4DBC 668F4DBC 1 ray-laptop 28918 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                         ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#define PK_ARRIVAL 		(op_intrpt_type () == OPC_INTRPT_STRM)


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
	Stathandle	             		ete_gsh                                         ;
	Stathandle	             		ete_lsh                                         ;
	} sink_w_delay2_state;

#define ete_gsh                 		op_sv_ptr->ete_gsh
#define ete_lsh                 		op_sv_ptr->ete_lsh

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	sink_w_delay2_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((sink_w_delay2_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


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
	void sink_w_delay2 (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_sink_w_delay2_init (int * init_block_ptr);
	void _op_sink_w_delay2_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_sink_w_delay2_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_sink_w_delay2_alloc (VosT_Obtype, int);
	void _op_sink_w_delay2_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
sink_w_delay2 (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (sink_w_delay2 ());

		{
		/* Temporary Variables */
		double		ete_delay;
		Packet*		pkptr;
		/* End of Temporary Variables */


		FSM_ENTER_NO_VARS ("sink_w_delay2")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (Init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "Init", "sink_w_delay2 [Init enter execs]")
				FSM_PROFILE_SECTION_IN ("sink_w_delay2 [Init enter execs]", state0_enter_exec)
				{
				/* Register handles for stat collection. */
				ete_gsh = op_stat_reg ("Global ETE delay (sec)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				ete_lsh = op_stat_reg ("ETE delay (sec)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (Init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "Init", "sink_w_delay2 [Init exit execs]")


			/** state (Init) transition processing **/
			FSM_TRANSIT_ONLY ((PK_ARRIVAL), 1, state1_enter_exec, ;, Init, "PK_ARRIVAL", "", "Init", "ETE_destroy", "tr_0", "sink_w_delay2 [Init -> ETE_destroy : PK_ARRIVAL / ]")
				/*---------------------------------------------------------*/



			/** state (ETE_destroy) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "ETE_destroy", state1_enter_exec, "sink_w_delay2 [ETE_destroy enter execs]")
				FSM_PROFILE_SECTION_IN ("sink_w_delay2 [ETE_destroy enter execs]", state1_enter_exec)
				{
				/* Get pointer to packet. */
				pkptr = op_pk_get (op_intrpt_strm ());
				
				/* Calc ETE delay. */
				ete_delay = op_sim_time () - op_pk_creation_time_get (pkptr);
				
				/* Write statistics. */
				op_stat_write (ete_gsh, ete_delay);
				op_stat_write (ete_lsh, ete_delay);
				
				/* Destroy packet. */
				op_pk_destroy (pkptr);
				
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"sink_w_delay2")


			/** state (ETE_destroy) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "ETE_destroy", "sink_w_delay2 [ETE_destroy exit execs]")


			/** state (ETE_destroy) transition processing **/
			FSM_TRANSIT_ONLY ((PK_ARRIVAL), 2, state2_enter_exec, ;, ETE_destroy, "PK_ARRIVAL", "", "ETE_destroy", "Wait", "tr_1", "sink_w_delay2 [ETE_destroy -> Wait : PK_ARRIVAL / ]")
				/*---------------------------------------------------------*/



			/** state (Wait) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "Wait", state2_enter_exec, "sink_w_delay2 [Wait enter execs]")
				FSM_PROFILE_SECTION_IN ("sink_w_delay2 [Wait enter execs]", state2_enter_exec)
				{
				
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (Wait) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "Wait", "sink_w_delay2 [Wait exit execs]")
				FSM_PROFILE_SECTION_IN ("sink_w_delay2 [Wait exit execs]", state2_exit_exec)
				{
				
				}
				FSM_PROFILE_SECTION_OUT (state2_exit_exec)


			/** state (Wait) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "Wait", "ETE_destroy", "tr_2", "sink_w_delay2 [Wait -> ETE_destroy : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"sink_w_delay2")
		}
	}




void
_op_sink_w_delay2_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_sink_w_delay2_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_sink_w_delay2_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_sink_w_delay2_svar function. */
#undef ete_gsh
#undef ete_lsh

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_sink_w_delay2_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_sink_w_delay2_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (sink_w_delay2)",
		sizeof (sink_w_delay2_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_sink_w_delay2_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	sink_w_delay2_state * ptr;
	FIN_MT (_op_sink_w_delay2_alloc (obtype))

	ptr = (sink_w_delay2_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "sink_w_delay2 [Init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_sink_w_delay2_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	sink_w_delay2_state		*prs_ptr;

	FIN_MT (_op_sink_w_delay2_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (sink_w_delay2_state *)gen_ptr;

	if (strcmp ("ete_gsh" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ete_gsh);
		FOUT
		}
	if (strcmp ("ete_lsh" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ete_lsh);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

