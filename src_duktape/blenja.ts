import { Entity } from "./Entity";

// blender\makesdna\DNA_object_types.h line 479
export var SELECT = 1;

export type U32 = number;
export type I32 = number;
export type Pointer = number;

// Generic
export declare function thingsHaveChanged();

// Mesh
export declare function mesh_add(): Pointer;
export declare function mesh_from_buffers(vertices: Float32Array, loops: Int32Array, polygons: Int32Array): Pointer;
export declare function mesh_rna_print();
export declare function mesh_update              (pointer: Pointer                                            );
export declare function mesh_totvert             (pointer: Pointer                                            ): I32;
export declare function mesh_totedge             (pointer: Pointer                                            ): I32;
export declare function mesh_totface             (pointer: Pointer                                            ): I32;
export declare function mesh_totselect           (pointer: Pointer                                            ): I32;
export declare function mesh_totpoly             (pointer: Pointer                                            ): I32;
export declare function mesh_totloop             (pointer: Pointer                                            ): I32;
export declare function mesh_rna_vertex_color_new(pointer: Pointer, name: string                              );
export declare function mesh_get_polyid_loopstart(pointer: Pointer, i: I32                                    );
export declare function mesh_get_polyid_totloop  (pointer: Pointer, i: I32                                    );
export declare function mesh_get_polyid_mat_nr   (pointer: Pointer, i: I32                                    );
export declare function mesh_get_polyid_flag     (pointer: Pointer, i: I32                                    );
export declare function mesh_set_polyid_loopstart(pointer: Pointer, i: I32, value: number                     );
export declare function mesh_set_polyid_totloop  (pointer: Pointer, i: I32, value: number                     );
export declare function mesh_set_polyid_mat_nr   (pointer: Pointer, i: I32, value: number                     );
export declare function mesh_set_polyid_flag     (pointer: Pointer, i: I32, value: number                     );
export declare function mesh_get_vertid_xyz      (pointer: Pointer, vertid: number, xyz: number               );
export declare function mesh_set_vertid_xyz      (pointer: Pointer, vertid: number, xyz: number, value: number);
export declare function mesh_get_loopid_ev       (pointer: Pointer, i: I32, ev: I32                           ): U32;
export declare function mesh_set_loopid_ev       (pointer: Pointer, i: I32, ev: I32, value: U32               ): I32;
export declare function mesh_get_edgeid_v1       (pointer: Pointer, i: I32                                    ): I32;
export declare function mesh_get_edgeid_v2       (pointer: Pointer, i: I32                                    ): I32;
export declare function mesh_set_edgeid_v1       (pointer: Pointer, i: I32, value: U32                        );
export declare function mesh_set_edgeid_v2       (pointer: Pointer, i: I32, value: U32                        );
export declare function _mesh_get_name           (pointer: Pointer                                            ): string;

// GHash

// Collection

// Entity / Object
export declare function _allEntities(): Pointer[];
export declare function new_object_with_mesh(name: string                       ): Pointer;
export declare function object_position     (pointer: Pointer                   ): ArrayBuffer;
export declare function object_reference_set(pointer: Pointer, reference: Entity);
export declare function object_destroy      (pointer: Pointer                   );
export declare function object_update       (pointer: Pointer                   );
export declare function _entity_select      (pointer: Pointer                   );
export declare function _entity_deselect    (pointer: Pointer                   );
export declare function _entity_jump_to     (pointer: Pointer                   );
export declare function object_children     (pointer: Pointer                   ): Pointer[];
export declare function object_name_get     (pointer: Pointer                   ): string;
export declare function object_mesh_get     (pointer: Pointer                   ): false | Pointer;

export declare function _selectedObjects(): Pointer[];
