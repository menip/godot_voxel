#include "voxel_tool_buffer.h"
#include "../voxel_buffer.h"

VoxelToolBuffer::VoxelToolBuffer(Ref<VoxelBuffer> vb) {
	ERR_FAIL_COND(vb.is_null());
	_buffer = vb;
}

bool VoxelToolBuffer::is_area_editable(const Rect3i &box) const {
	ERR_FAIL_COND_V(_buffer.is_null(), false);
	return Rect3i(Vector3i(), _buffer->get_size()).encloses(box);
}

int VoxelToolBuffer::_get_voxel(Vector3i pos) {
	ERR_FAIL_COND_V(_buffer.is_null(), 0);
	return _buffer->get_voxel(pos, _channel);
}

float VoxelToolBuffer::_get_voxel_f(Vector3i pos) {
	ERR_FAIL_COND_V(_buffer.is_null(), 0);
	return _buffer->get_voxel_f(pos.x, pos.y, pos.z, _channel);
}

void VoxelToolBuffer::_set_voxel(Vector3i pos, int v) {
	ERR_FAIL_COND(_buffer.is_null());
	return _buffer->set_voxel(v, pos, _channel);
}

void VoxelToolBuffer::_set_voxel_f(Vector3i pos, float v) {
	ERR_FAIL_COND(_buffer.is_null());
	return _buffer->set_voxel_f(v, pos.x, pos.y, pos.z, _channel);
}

void VoxelToolBuffer::_post_edit(const Rect3i &box) {
	ERR_FAIL_COND(_buffer.is_null());
	// Nothing special to do
}

void VoxelToolBuffer::paste(Vector3i p_pos, Ref<VoxelBuffer> p_voxels, int mask_value) {
	ERR_FAIL_COND(_buffer.is_null());
	ERR_FAIL_COND(p_voxels.is_null());

	VoxelBuffer *dst = *_buffer;
	const VoxelBuffer *src = *p_voxels;

	Rect3i box(p_pos, p_voxels->get_size());
	const Vector3i min_noclamp = box.pos;
	box.clip(Rect3i(Vector3i(), _buffer->get_size()));

	const int channel = get_channel();

	const Vector3i box_max = box.pos + box.size;

	for (int z = box.pos.z; z < box_max.z; ++z) {
		const int bz = z - min_noclamp.z;

		for (int x = box.pos.x; x < box_max.x; ++x) {
			const int bx = x - min_noclamp.x;

			for (int y = box.pos.y; y < box_max.y; ++y) {
				const int by = y - min_noclamp.y;

				const uint64_t v = src->get_voxel(bx, by, bz, channel);
				if (v != mask_value) {
					dst->set_voxel(v, x, y, z, channel);
				}
			}
		}
	}
}
