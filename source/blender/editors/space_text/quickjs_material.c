I managed to create the material and assign it as texture:

Material *mat = BKE_material_add( bmain, DATA_( "Material" ) );

ED_node_shader_default( C, &mat->id );

assign_material( bmain, ob_mesh, mat, ob_mesh->actcol, BKE_MAT_ASSIGN_EXISTING );
By doing it manually on Blender, I’ve undestood that I also need to use nodes and use as “Base Color” an “Image Texture”, and I’ve managed to do this with this code:

mat->use_nodes = true;

bNode *imanode;
bNodeTree *ntree = mat->nodetree;

imanode = nodeAddStaticNode( C, ntree, SH_NODE_TEX_IMAGE );

imanode->id = &image->id;

nodeSetActive( ntree, imanode );

bNode *in_node = ntreeFindType( ntree, SH_NODE_BSDF_PRINCIPLED );
bNode *out_node = imanode;

if ( in_node != NULL )
{
	bNodeSocket *out_sock = nodeFindSocket( out_node, SOCK_OUT, "Color" );
	bNodeSocket *in_sock = nodeFindSocket( in_node, SOCK_IN, "Base Color" );

	bNodeLink *link = in_sock ? in_sock->link : NULL;
	if ( in_sock != NULL && link == NULL )
	{
		nodeAddLink( ntree, out_node, out_sock, in_node, in_sock );

		nodePositionRelative( out_node, in_node, out_sock, in_sock );
	}
}

ntreeUpdateTree( CTX_data_main( C ), ntree );

However, this is not enough. I need also to set “Vector” in “Base Color” as “Texture Coordinates | Generated”. I can do that manually, but I cannot understand how to do that with the code. Any suggestion?



Those are nodes, you need to add a TextureCoordinate node (SH_NODE_TEX_COORD I believe), and link its Generated output to the Vector input of the Image texture node.

Yeah I managed to do it yesterday :slight_smile:

I made it this way:

bNode *tex_node = nodeAddNode( C, ntree, "ShaderNodeTexCoord" );

if ( tex_node != NULL )
{
	bNodeSocket *out_sock = nodeFindSocket( tex_node, SOCK_OUT, "Generated" );
	bNodeSocket *in_sock = nodeFindSocket( ima_node, SOCK_IN, "Vector" );

	/* Check if the socket in already connected to something */
	bNodeLink *link = in_sock ? in_sock->link : NULL;
	if ( in_sock != NULL && link == NULL )
	{
		nodeAddLink( ntree, tex_node, out_sock, ima_node, in_sock );

		nodePositionRelative( tex_node, ima_node, out_sock, in_sock );
	}
}