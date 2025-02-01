# Report

## Part (C)

(run on SCC 4-core VSCode server)

Average over 5 runs (in microseconds):

- W1: 2.471956e+07
- W2: 3.800794e+06
- W3: 5.946408e+06
- W4: 4.66795e+07
- W5: 9.514118e+06
- W6: 1.333208e+07

Raw data:

- W1: 2.44891e+07, 2.49367e+07, 2.47315e+07, 2.43807e+07, 2.50598e+07
- W2: 3.75066e+06, 3.74087e+06, 3.8148e+06, 3.86768e+06, 3.82996e+06
- W3: 6.03268e+06, 5.95989e+06, 6.11625e+06, 5.79207e+06, 5.83115e+06
- W4: 4.5354e+07, 4.78357e+07, 4.74252e+07, 4.66414e+07, 4.61412e+07
- W5: 9.63638e+06, 9.23076e+06, 9.43182e+06, 9.52973e+06, 9.7419e+06
- W6: 1.34216e+07, 1.32799e+07, 1.34083e+07, 1.32269e+07, 1.33237e+07

## Part (D)

### (i)
- Making the following assumptions:
  - `int`s and `uint`s are 4 bytes
  - pointers are 2 bytes
  - vectors incur no memory overhead (besides the pointer to a contiguous array in memory)
- All $N$ integer elements comprise $4*N$ bytes of footprint
- Each zone contains, besides the elements themselves, 14 bytes of extra data
  - 4 bytes for each of the min, max, and size fields
  - 2 bytes for the pointer to the elements
- There are $N/d$ zones, so our zones take up $14N/d$ bytes in total (excluding elements)
  - Including all elements across all zones, we take up $14N/d + 4N$ bytes
- A zonemap has 4 fields:
  - elements is $4*N$ bytes plus 2 bytes for the vector
  - zones is $14N/d + 4N$ bytes, as above
  - num_zones and num_elements_per_zone are 4 bytes each
- In total, a zonemap of ints takes $8N+14N/d+10$ bytes of memory footprint.
  - We can cut this to $4N+14N/d+10$ bytes by not storing the elements in the zonemap object itself, but only in the zones.


### (ii)
- We would only want to send zones of elements to disk when absolutely necessary due to slower speeds.
- Assuming we can keep some zones in memory but not others, we would want to keep those which are accessed most frequently.
  - This would be determined via analyzing actual usage of the system.
- In lieu of (or in conjunction with) this information, we could prioritize zones with larger ranges.
  - Assuming that queries are uniformly distributed, zones with larger ranges will be accessed more, and so it would be more efficient to keep these zones in memory.
    - But this is a big assumption, and I would guess that it will not hold in practice most of the time.