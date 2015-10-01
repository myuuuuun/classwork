import std.stdio;
import std.math;
import std.bitmanip : BitArray;
import std.datetime : StopWatch;
import core.bitop   : bts, popcnt, bt;

enum int n = 180_000_000;
enum Leng = n/3 + (n%6 == 2);

void main(){
    StopWatch sw;
    sw.start;
    
    int k, k2;
    BitArray sieve;
    sieve.length = Leng;
    size_t* p = sieve.ptr;
    
    int cnt = 1, idx;
    
    foreach(i; 1..cast(uint)((n^^0.5)/3 + 1)){
        k = (3 * i + 1) | 1;
        k2 = k << 1;
        
        for(int j = k*k/3; j < Leng; j += k2)
            bts(p, j);
        for(int j = k*(k-((i&1)<<1)+4)/3; j < Leng; j += k2)
            bts(p, j);
    }
    
    
    
    LOOP:
    foreach(e; p[0..sieve.dim]){
        
        cnt += 32 - popcnt(e);
        
        if(cnt >= 10_000_000){
            cnt -= 32 - popcnt(e);
            idx <<= 5;
            
            for(;;++idx){
                cnt += !bt(p, idx);
                
                if(cnt == 10_000_000){
                    writeln((3 * idx + 1)|1);
                    sw.stop;
                    break LOOP;
                }
            }
        }
        ++idx;
    }
    
    writeln(sw.peek.msecs);
}