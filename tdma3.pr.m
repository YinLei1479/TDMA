MIL_3_Tfile_Hdr_ 145A 140A opnet 9 363A0214 668F868A 2B ray-laptop 28918 0 0 none none 0 0 none AB2477AE 3DCC 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                ��g�      @   D   �  �  �  �  1N  1R  6  ;�  ;�  ;�  ;�  �          Slot Length   �������      seconds       ?�      ����              ����              ����           �Z             	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����          6   int	\my_offset;       double	\slot_length;       double	\tx_data_rate;       int	\intrpt_flag;       int	\num_pk_sent;       int	\num_pk_rcvd;       int	\num_bits_sent;       int	\num_bits_rcvd;       Stathandle	\num_pk_sent_stat;        Stathandle	\global_pk_sent_stat;       Stathandle	\num_pk_rcvd_stat;        Stathandle	\global_pk_rcvd_stat;       Objid	\my_node_id;       Objid	\my_id;       Stathandle	\num_bits_sent_stat;       "Stathandle	\global_bits_sent_stat;       Stathandle	\num_bits_rcvd_stat;       "Stathandle	\global_bits_rcvd_stat;       Stathandle	\bits_sec_rcvd_stat;       Stathandle	\bits_sec_sent_stat;       Stathandle	\pk_sec_rcvd_stat;       Stathandle	\pk_sec_sent_stat;       &Stathandle	\global_bits_sec_rcvd_stat;       &Stathandle	\global_bits_sec_sent_stat;       $Stathandle	\global_pk_sec_rcvd_stat;       $Stathandle	\global_pk_sec_sent_stat;       int	\num_slots;       
   Objid tx_id,comp_id,tx_ch_id;   Packet* pkptr;   int pk_len;   double current_time;   int used_slots;   int current_offset;   double time_left_in_slot;   double pk_time;   double next_offset;   double my_next_slot_time;      #include <math.h>       /* Constant Definitions */   #define RX_IN_STRM		(1)   #define SRC_IN_STRM		(0)   #define TX_OUT_STRM		(1)   #define SINK_OUT_STRM	(0)       7#define EPSILON  		(1e-10)  /* rounding error factor */   #define TDMA_COMPLETE	(-10)           "/* Transition Condition Macros */    \#define FROM_RX			(op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm () == RX_IN_STRM)   _#define FROM_SRC 		(op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm () == SRC_IN_STRM)    5#define TRANSMITTING	(op_stat_local_read (0) == 1.0)    5#define SLOT 			(op_intrpt_type() == OPC_INTRPT_SELF)   5#define END  			(op_intrpt_type() == OPC_INTRPT_STAT)   )#define DATA_ENQ 		(!(op_subq_empty (0)))       ,#define	SELF_INTRPT_SCHLD	(intrpt_flag == 1)       /* Global Variables */   int		tdma_pk_sent;   int		tdma_pk_rcvd;   int		tdma_bits_sent;   int		tdma_bits_rcvd;   int		tdma_setup;   int		tdma_id;   int		num_slots;          Iprintf ("Object ID = %d Current Sim Time = %g\n", my_id, op_sim_time ());   ,printf ("My TDMA Offset = %d\n", my_offset);   2printf ("Number of TDMA Slots = %d\n", num_slots);   :printf ("Number of Packets Received = %d\n", num_pk_rcvd);   9printf ("Number of Bits Received = %d\n", num_bits_rcvd);   6printf ("Number of Packets Sent = %d\n", num_pk_sent);   5printf ("Number of Bits Sent = %d\n", num_bits_sent);                                       Z   �          
   init   
       
   C   '/* Get the slot length for all nodes */   Bop_ima_sim_attr_get (OPC_IMA_DOUBLE, "Slot Length", &slot_length);       my_id = op_id_self();   $my_node_id = op_topo_parent (my_id);       4/* Initialize tdma offsets if not done previously */    if (tdma_setup != TDMA_COMPLETE)   {   	num_slots = 0;   	tdma_setup = TDMA_COMPLETE;   }       (/* Calculate the offset for this node */   num_slots++;   my_offset = num_slots-1;	       &if (op_prg_odb_ltrace_active ("tdma"))   {   *	printf ("Node Objid = %d\n", my_node_id);   *	printf ("Node Offset = %d\n", my_offset);   	printf ("\n");   }   	   C/*Determine the number of various types of nodes in the network */    1/* Determine the data rate for the transmitter */   Itx_id =  op_topo_assoc (my_id, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_RATX, 0);    5comp_id = op_topo_child (tx_id, OPC_OBJTYPE_COMP, 0);   :tx_ch_id = op_topo_child (comp_id, OPC_OBJTYPE_RATXCH, 0);   <op_ima_obj_attr_get (tx_ch_id, "data rate", &tx_data_rate);        0/* Initialize statistic calculation variables */       /*   tdma_pk_sent = 0;   tdma_pk_rcvd = 0;   tdma_bits_sent = 0;   tdma_bits_rcvd = 0;   num_pk_sent = 0;   num_pk_rcvd = 0;   num_bits_sent = 0;   num_bits_rcvd = 0;   */       /* Register Statistics */   \num_pk_sent_stat = op_stat_reg ("TDMA.Load (packets)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   eglobal_pk_sent_stat = op_stat_reg ("TDMA.TDMA Load (packets)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);   hnum_pk_rcvd_stat = op_stat_reg ("TDMA.Traffic Received (packets)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   qglobal_pk_rcvd_stat = op_stat_reg ("TDMA.TDMA Traffic Received (packets)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);       [num_bits_sent_stat = op_stat_reg ("TDMA.Load (bits)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   dglobal_bits_sent_stat = op_stat_reg ("TDMA.TDMA Load (bits)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);   gnum_bits_rcvd_stat = op_stat_reg ("TDMA.Traffic Received (bits)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   pglobal_bits_rcvd_stat = op_stat_reg ("TDMA.TDMA Traffic Received (bits)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);       kbits_sec_rcvd_stat = op_stat_reg ("TDMA.Traffic Received (bits/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   _bits_sec_sent_stat = op_stat_reg ("TDMA.Load (bits/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   lpk_sec_rcvd_stat = op_stat_reg ("TDMA.Traffic Received (packets/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   `pk_sec_sent_stat = op_stat_reg ("TDMA.Load (packets/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);       xglobal_bits_sec_rcvd_stat = op_stat_reg ("TDMA.TDMA Traffic Received (bits/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);   vglobal_pk_sec_rcvd_stat = op_stat_reg ("TDMA.TDMA Traffic Received (bits/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);   mglobal_pk_sec_sent_stat = op_stat_reg ("TDMA.TDMA Load (packets/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);   oglobal_bits_sec_sent_stat = op_stat_reg ("TDMA.TDMA Load (packets/sec)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);       D/* Schedule interupt to complete initialization in the exit execs */   ,op_intrpt_schedule_self (op_sim_time (), 0);   
       
          
       
    ����   
          pr_state      	     �          
   idle   
       
       
       
       
       
    ����   
          pr_state           Z          
   fr_rx   
       
        pkptr =  op_pk_get (RX_IN_STRM);       /pk_len = (double) op_pk_total_size_get (pkptr);       /** Record Statistics **/   ?/** The bits/sec or packets/sec statistics are recorded in		**/   @/** bits and packets, and then the OPNET statistic "capture		**/   =/** mode" is used to obtain a bucketized sum over time.			**/   A/** Record extra 0.0 data-points to enable proper computation	**/   1/** of the "sum/time" based statistics.							**/       &op_stat_write (num_pk_rcvd_stat, 1.0);   &op_stat_write (pk_sec_rcvd_stat, 1.0);   &op_stat_write (pk_sec_rcvd_stat, 0.0);       )op_stat_write (global_pk_rcvd_stat, 1.0);   -op_stat_write (global_pk_sec_rcvd_stat, 1.0);   -op_stat_write (global_pk_sec_rcvd_stat, 0.0);       +op_stat_write (num_bits_rcvd_stat, pk_len);   +op_stat_write (bits_sec_rcvd_stat, pk_len);   (op_stat_write (bits_sec_rcvd_stat, 0.0);       .op_stat_write (global_bits_rcvd_stat, pk_len);   2op_stat_write (global_bits_sec_rcvd_stat, pk_len);   /op_stat_write (global_bits_sec_rcvd_stat, 0.0);       "op_pk_send (pkptr, SINK_OUT_STRM);                   
       
       
       
   ����   
          pr_state        �   �          
   fr_src   
       
      !pkptr =  op_pk_get (SRC_IN_STRM);   -op_subq_pk_insert (0, pkptr, OPC_QPOS_TAIL);    
       
       
       
   ����   
          pr_state          �          
   tx   
       
   U   current_time = op_sim_time();       %/* Determine if currently my slot. */   )/* EPSILON accounts for rounding error */   Bused_slots = (int) floor ((current_time / slot_length) + EPSILON);       (current_offset = used_slots % num_slots;       ?/* See if there is time left in the current slot to transmit */   (/* the packet at the top of the queue */   Btime_left_in_slot = ((used_slots + 1)*slot_length) - current_time;       Opk_len = (double) op_pk_total_size_get (op_subq_pk_access (0, OPC_QPOS_HEAD));    )pk_time = (double) pk_len / tx_data_rate;       ?/* If this is my slot and I have enough time to transmit the */   ?/* entire packet then transmit. Otherwise set a self intrpt  */   ?/* for the beginning of my next slot.                        */   Cif ((current_offset == my_offset) && (pk_time < time_left_in_slot))   	{       %	/* dequeue the packet and send it */   .	pkptr = op_subq_pk_remove (0, OPC_QPOS_HEAD);       2	/* reset the flag to schedule a self interrupt */   2	/* for packets arriving subsequent to this one */   	intrpt_flag = 0;       '	if (op_prg_odb_ltrace_active ("tdma"))   	{   T		printf ("TDMA Node %d is transmitting at time: %f\n", my_node_id, op_sim_time ());   		printf ("\n");   	}   	   0	pk_len = (double) op_pk_total_size_get (pkptr);       	/** Record Statistics **/   @	/** The bits/sec or packets/sec statistics are recorded in		**/   A	/** bits and packets, and then the OPNET statistic "capture		**/   >	/** mode" is used to obtain a bucketized sum over time.			**/   B	/** Record extra 0.0 data-points to enable proper computation	**/   2	/** of the "sum/time" based statistics.							**/       '	op_stat_write (num_pk_sent_stat, 1.0);   '	op_stat_write (pk_sec_sent_stat, 1.0);   '	op_stat_write (pk_sec_sent_stat, 0.0);       *	op_stat_write (global_pk_sent_stat, 1.0);   .	op_stat_write (global_pk_sec_sent_stat, 1.0);   .	op_stat_write (global_pk_sec_sent_stat, 0.0);       ,	op_stat_write (num_bits_sent_stat, pk_len);   ,	op_stat_write (bits_sec_sent_stat, pk_len);   )	op_stat_write (bits_sec_sent_stat, 0.0);       /	op_stat_write (global_bits_sent_stat, pk_len);   3	op_stat_write (global_bits_sec_sent_stat, pk_len);   0	op_stat_write (global_bits_sec_sent_stat, 0.0);       !	op_pk_send (pkptr, TX_OUT_STRM);       	} /* End if */   else   	{       *	next_offset = my_offset - current_offset;       	if (next_offset <= 0)   		{   		next_offset += num_slots;   		}           G	my_next_slot_time = (double) (used_slots + next_offset) * slot_length;       4	/* schedule a self interrupt only if one has not */   4	/* been already scheduled for the next slot time */   	if (!SELF_INTRPT_SCHLD)   		{   2		op_intrpt_schedule_self (my_next_slot_time, 0);    		intrpt_flag = 1;   		}   	} /* End else */           
       
       
       
   ����   
          pr_state         	      	   	   �   �      �   �   �   �   �   �   �   �          
   tr_10   
       
   default   
       
����   
       
    ����   
       
   ����   
                    pr_transition            	   �   �      v   �   �   �          
   tr_12   
       
����   
       
����   
       
    ����   
       
   ����   
                    pr_transition         	      �   �      �   �   �   o          
   tr_13   
       
   FROM_RX   
       
����   
       
    ����   
       
   ����   
                    pr_transition            	  "   �     "   m  "   �          
   tr_14   
       
����   
       
����   
       
    ����   
       
   ����   
                    pr_transition      #   	     m   �     %   �  �   �          
   tr_15   
       
   FROM_SRC   
       
����   
       
    ����   
       
   ����   
                    pr_transition      '      	  m   �     �   �  '   �          
   tr_16   
       
   TRANSMITTING   
       
����   
       
    ����   
       
   ����   
                    pr_transition      +      	   �  +      �  n   �   �          
   tr_18   
       
����   
       
����   
       
    ����   
       
   ����   
                    pr_transition      /   	     r  &        �    l          
   tr_19   
       
   (END || SLOT) && DATA_ENQ   
       
����   
       
    ����   
       
   ����   
                    pr_transition      3        �  x     �   �  �  U  *  �          
   tr_20   
       
   !TRANSMITTING   
       
����   
       
    ����   
       
   ����   
                    pr_transition         6          Load (bits)          (Number of bits broadcast by this node.          TDMA   bucket/default total/sum   linear        ԲI�%��}   Load (bits/sec)          'Number of bits per second broadcast by    this node.      TDMA   bucket/default total/sum_time   linear        ԲI�%��}   Load (packets)          $Number of packets broadcast by this    node.      TDMA   bucket/default total/sum   linear        ԲI�%��}   Load (packets/sec)          'Number of packets per second broadcast    by this node.      TDMA   bucket/default total/sum_time   linear        ԲI�%��}   Traffic Received (bits)          'Number of bits received by this node.     TDMA   bucket/default total/sum   linear        ԲI�%��}   Traffic Received (bits/sec)          &Number of bits per second received by    this node.      TDMA   bucket/default total/sum_time   linear        ԲI�%��}   Traffic Received (packets)          #Number of packets received by this    node.    TDMA   bucket/default total/sum   linear        ԲI�%��}   Traffic Received (packets/sec)          &Number of packets per second received    by this node.       TDMA   bucket/default total/sum_time   linear        ԲI�%��}      TDMA Load (bits)          &Total number of bits broadcast by all    tdma capable nodes.     TDMA   bucket/default total/sum   linear        ԲI�%��}   TDMA Load (bits/sec)           Total number of bits per second    &broadcast by all tdma capable nodes.     TDMA   bucket/default total/sum_time   linear        ԲI�%��}   TDMA Load (packets)          #Total number of packets per second    %broadcast by all tdma capable nodes.    TDMA   bucket/default total/sum_time   linear        ԲI�%��}   TDMA Load (packets/sec)          #Total number of packets per second    %broadcast by all tdma capable nodes.    TDMA   bucket/default total/sum_time   linear        ԲI�%��}   TDMA Traffic Received (bits)           Total number of bits per second    $received by all tdma capable nodes.    TDMA   bucket/default total/sum_time   linear        ԲI�%��}    TDMA Traffic Received (bits/sec)           Total number of bits per second    $received by all tdma capable nodes.    TDMA   bucket/default total/sum_time   linear        ԲI�%��}   TDMA Traffic Received (packets)          (Total number of packets received by all    tdma capable nodes.    TDMA   bucket/default total/sum   linear        ԲI�%��}   #TDMA Traffic Received (packets/sec)          #Total number of packets per second    $received by all tdma capable nodes.    TDMA   bucket/default total/sum_time   linear        ԲI�%��}                           