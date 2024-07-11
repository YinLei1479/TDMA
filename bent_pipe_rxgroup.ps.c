/* bent_pipe_rxgroup.ps.c */                                                       
/* Bent pipe receiver group model for radio link Transceiver Pipeline */

/****************************************/
/*		  Copyright (c) 1995			*/
/*			by MIL 3, Inc.				*/
/*		(A Delaware Corporation)		*/
/*	3400 International Drive,  N.W.		*/
/*		Washington, D.C., U.S.A.		*/
/*			All Rights Reserved.		*/
/****************************************/

#include <opnet.h>


int
bent_pipe_rxgroup (txch_obid, rxch_obid)	
	Objid		txch_obid, rxch_obid;
	{
	Objid		tx_obid, rx_obid;
	Objid		tx_node_obid, rx_node_obid;
	Objid		rx_subnet_obid;
	char		rx_subnet_name[64];
	char		tx_name[64];
	int			tx_node_type, rx_node_type;

	/** Determine the potential for communication between	**/
	/** given transmitter and receiver channel objects.		**/
	FIN (bent_pipe_rxgroup (txch_obid, rxch_obid))

	tx_obid = op25_topo_parent (txch_obid);
	rx_obid = op25_topo_parent (rxch_obid);
	tx_node_obid = op25_topo_parent (tx_obid);
	rx_node_obid = op25_topo_parent (rx_obid);
	tx_node_type = op_id_to_type (tx_node_obid);
	rx_node_type = op_id_to_type (rx_node_obid);

	/* transmitters in ground nodes may only communicate */
	/* with receivers in the satellite */
	if (tx_node_type == OPC_OBJTYPE_NDFIX)
		{
		if (rx_node_type == OPC_OBJTYPE_NDSAT)
			FRET (OPC_TRUE)
		else
			FRET (OPC_FALSE)	
		}

	else
		{
		/* transmitters in the satellite may only communicate with */
		/* receivers in the appropriate subnet */		
		rx_subnet_obid = op25_topo_parent (rx_node_obid);
		op_ima_obj_attr_get (rx_subnet_obid, "name", rx_subnet_name);
		op_ima_obj_attr_get (tx_obid, "name", tx_name);
		if (!strcmp (rx_subnet_name, "west_net") && !strcmp (tx_name, "w_rt_0"))
			FRET (OPC_TRUE)
		else	
			if (!strcmp (rx_subnet_name, "east_net") && !strcmp (tx_name, "e_rt_1"))
				FRET (OPC_TRUE)
			else
				FRET (OPC_FALSE)	
		}	
	
	}                
