MIL_3_Tfile_Hdr_ 100A 94E op_cvmod 9 349720A4 4133E3A3 5 opnet chunming 0 0 none none 0 0 none 70C8F38A C9F 0 0 0 0 0 0 8f3 1                                                                                                                                                                                                                                                                                                                                                                                                   ��g�      @   D   H  9  =    �  �  	�  
�  
�  
�  
�  �           
   begsim intrpt             ����      doc file            	nd_module      doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             Stathandle		\ete_gsh;   Stathandle		\ete_lsh;      double		ete_delay;   Packet*		pkptr;      ;#define PK_ARRIVAL 		(op_intrpt_type () == OPC_INTRPT_STRM)                                         �   �             Init          
      +/* Register handles for stat collection. */   Wete_gsh = op_stat_reg ("Global ETE delay (sec)", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);   Oete_lsh = op_stat_reg ("ETE delay (sec)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);       
                        ����             pr_state      	  J   �             ETE_destroy          
      /* Get pointer to packet. */   &pkptr = op_pk_get (op_intrpt_strm ());       /* Calc ETE delay. */   =ete_delay = op_sim_time () - op_pk_creation_time_get (pkptr);       /* Write statistics. */   #op_stat_write (ete_gsh, ete_delay);   #op_stat_write (ete_lsh, ete_delay);       /* Destroy packet. */   op_pk_destroy (pkptr);       
                     
    ����   
          pr_state        J  J             Wait          
          
       
          
       
   ����   
          pr_state                  	   �   }      �   �   �   �  .   �             tr_0             
PK_ARRIVAL          ����          
    ����   
          ����                       pr_transition         	     �   �     Y   �  e   �  V  /             tr_1             
PK_ARRIVAL          ����          
    ����   
          ����                       pr_transition            	  :  
     C  -  2   �  =   �             tr_2          ����          ����          
    ����   
          ����                       pr_transition                   ETE delay (sec)          #End-to-end delay.  Measured by the    $current sim time minus the creation    "time of the packet for this node. ����   normal   linear        ԲI�%��}      Global ETE delay (sec)          #End-to-end delay.  Measured by the    $current sim time minus the creation    (time of the packet for all nodes in the    	network.
   
����   normal   discrete        ԲI�%��}                           