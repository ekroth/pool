__kernel void hello(__global float * out)
{
    size_t tid = get_global_id(0);
    out[tid] = native_cos(out[tid]);
}