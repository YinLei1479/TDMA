MIL_3_Tfile_Hdr_ 51D 50A op_cvmod 9 349720A4 35D33EBD 2 shawkings wwhimpenny 0 0 none none 0 0 none 22509B41 1193 0 0 0 0                                                                                                                                                                                                                                                                                                                                                                                                       ��g�      8   <   @        H  L  {    �  �           	   begsim intrpt             ����      doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����                 Objid w_cent_id;   Objid w_sub_id;   Objid w_tx_id;   Objid e_tx_id;   Objid e_sub_id;   Objid e_cent_id;   Objid e_ant_id;   Objid w_ant_id;   Packet*  pkptr;   Objid my_id;   double lat, lng, alt, x, y, z;      #define W_IN_STRM 0   #define E_IN_STRM 1   #define W_OUT_STRM 0   #define E_OUT_STRM 1   0#define FROM_E (op_intrpt_strm () == E_IN_STRM)    0#define FROM_W (op_intrpt_strm () == W_IN_STRM)    8#define BEG_SIM (op_intrpt_type () == OPC_INTRPT_BEGSIM)                         Z   �          
   init   
       
       /* obtain id of this module */   my_id = op_id_self ();       %/* obtain the tx id, to get ant id */       Sw_tx_id = op_topo_assoc (my_id, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_RATX, W_OUT_STRM);    Se_tx_id = op_topo_assoc (my_id, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_RATX, E_OUT_STRM);        /* get ant id */       Lw_ant_id = op_topo_assoc (w_tx_id, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_ANT, 0);    Le_ant_id = op_topo_assoc (e_tx_id, OPC_TOPO_ASSOC_OUT, OPC_OBJTYPE_ANT, 0);        /* obtain id of center nodes */       @w_sub_id = op_id_from_name (0, OPC_OBJTYPE_SUBNET, "west_net");    @e_sub_id = op_id_from_name (0, OPC_OBJTYPE_SUBNET, "east_net");    Dw_cent_id = op_id_from_name (w_sub_id, OPC_OBJTYPE_NDFIX, "gnd_0");    Ce_cent_id = op_id_from_name (e_sub_id, OPC_OBJTYPE_NDFIX, "gnd_0");       N/* determine position of center nodes and point the antennas to those nodes */       ?op_ima_node_pos_get (w_cent_id, &lat, &lng, &alt, &x, &y, &z);    8op_ima_obj_attr_set (w_ant_id, "target altitude", alt);    9op_ima_obj_attr_set (w_ant_id, "target longitude", lng);    7op_ima_obj_attr_set (w_ant_id, "target latitude", lat);       ?op_ima_node_pos_get (e_cent_id, &lat, &lng, &alt, &x, &y, &z);    8op_ima_obj_attr_set (e_ant_id, "target altitude", alt);    9op_ima_obj_attr_set (e_ant_id, "target longitude", lng);    7op_ima_obj_attr_set (e_ant_id, "target latitude", lat);       
       
       
       
   ����   
          pr_state      	     �          
   idle   
       
       
       
      $pkptr = op_pk_get(op_intrpt_strm());   
       
    ����   
          pr_state        �             
   from W   
       
      op_pk_send (pkptr, E_OUT_STRM);       
       
       
       
   ����   
          pr_state        �            
   from E   
       
      op_pk_send (pkptr, W_OUT_STRM);       
       
       
       
   ����   
          pr_state                  	   �   �      v   �   �   �          
   tr_0   
       
   BEG_SIM   
       
����   
       
    ����   
       
   ����   
                    pr_transition         	     X   C     $   �  �   (          
   tr_1   
       
   FROM_W   
       
����   
       
    ����   
       
   ����   
                    pr_transition            	  o   g     �   6  *   �          
   tr_2   
       
����   
       
����   
       
    ����   
       
   ����   
                    pr_transition         	     �   �     &   �  �   �          
   tr_3   
       
   FROM_E   
       
����   
       
    ����   
       
   ����   
                    pr_transition            	  `   �     �       �          
   tr_4   
       
����   
       
����   
       
    ����   
       
   ����   
                    pr_transition                    outstat   d    ����   normal   linear                       