#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

struct block {
  unsigned long long int tag;
  int valid;
  int count;
};

struct set {
  struct block *blocks;
};

struct cache {
  struct set *sets;
};

int simulatefifo(struct cache*, unsigned long long int, int, int, char*, char*);
int prefetchfifo(struct cache*, unsigned long long int, unsigned long long int, int, int, int, char*, char*);
void prefetcher(struct cache*, unsigned long long int, int, int, char*, char*);

int pr = 0;
long Count = 0;

int simulatefifo(struct cache *Cache, unsigned long long int tag, int i_bit, int assoc, char *policy, char *type){
  for (int i = 0; i < assoc; i++){
    if (Cache->sets[i_bit].blocks[i].valid == 0){
      Count++;
      Cache->sets[i_bit].blocks[i].valid = 1;
      Cache->sets[i_bit].blocks[i].tag = tag;
      Cache->sets[i_bit].blocks[i].count = Count;
      return 0;
    } else {
      if (Cache->sets[i_bit].blocks[i].valid == 1) {
	if (Cache->sets[i_bit].blocks[i].tag == tag){
	  
	  if (!strcmp(policy, "lru")){
	    Count++;
	    Cache->sets[i_bit].blocks[i].count = Count;
	    return 1;
	  }
	  
	  return 1;
	} else {
	  if (i == (assoc-1)){
	    Count++;
	    if (!strcmp(policy, "direct")){
	      Cache->sets[i_bit].blocks[i].valid = 1;
	      Cache->sets[i_bit].blocks[i].tag = tag;
	      Cache->sets[i_bit].blocks[i].count = Count;
	      return 0;
	    } else {
	      //finds the mimimum 
	      int smallelem = Cache->sets[i_bit].blocks[0].count;
	      int smallindex;
	      for (int j = 0; j < assoc; j++){
		if (Cache->sets[i_bit].blocks[j].count <= smallelem){
		  smallelem = Cache->sets[i_bit].blocks[j].count;
		  smallindex = j;
		}
	      }
	      Cache->sets[i_bit].blocks[smallindex].valid = 1;
	      Cache->sets[i_bit].blocks[smallindex].tag = tag;
	      Cache->sets[i_bit].blocks[smallindex].count = Count;
	      return 0;
	    }
	  }
	}
      }
    }
  }
  return 0;
}

int prefetchfifo(struct cache *Pcache, unsigned long long int tag, unsigned long long int ptag, int i_bit, int assoc, int pi_bit, char *policy, char *type){
  for (int i = 0; i < assoc; i++){
    if (Pcache->sets[i_bit].blocks[i].valid == 0){
      Count++;
      Pcache->sets[i_bit].blocks[i].valid = 1;
      Pcache->sets[i_bit].blocks[i].tag = tag;
      Pcache->sets[i_bit].blocks[i].count = Count;
      prefetcher(Pcache, ptag, pi_bit, assoc, policy, type);
      return 0;
    } else {
      if (Pcache->sets[i_bit].blocks[i].valid == 1) {
	if (Pcache->sets[i_bit].blocks[i].tag == tag){
	  if (!strcmp(policy, "lru")){
	    Count++;
	    Pcache->sets[i_bit].blocks[i].count = Count;
	    return 1;
	  }
	  return 1;
	} else {
	  if (i == (assoc-1)){
	    Count++;
	    if (!strcmp(type, "direct")){
	      Pcache->sets[i_bit].blocks[i].valid = 1;
	      Pcache->sets[i_bit].blocks[i].tag = tag;
	      Pcache->sets[i_bit].blocks[i].count = Count;
	      prefetcher(Pcache, ptag, pi_bit, assoc, policy, type);
	      return 0;
	    } else {
	      //finds the mimimum 
	      int smallelem = Pcache->sets[i_bit].blocks[0].count;
	      int smallindex;
	      for (int j = 0; j < assoc; j++){
		if (Pcache->sets[i_bit].blocks[j].count <= smallelem){
		  smallelem = Pcache->sets[i_bit].blocks[j].count;
		  smallindex = j;
		}
	      }
	      Pcache->sets[i_bit].blocks[smallindex].valid = 1;
	      Pcache->sets[i_bit].blocks[smallindex].tag = tag;
	      Pcache->sets[i_bit].blocks[smallindex].count = Count;
	      prefetcher(Pcache, ptag, pi_bit, assoc, policy, type);
	      return 0;
	    }
	  }
	}   
      }
    }
  }
  return 0;
  
}

void prefetcher(struct cache * pcache, unsigned long long int ptag, int pi_bit, int assoc, char *policy, char  *type) {
  // printf("%s\n", policy);
  for (int i = 0; i < assoc; i++){
    if (pcache->sets[pi_bit].blocks[i].valid == 0){
      Count++;
      pcache->sets[pi_bit].blocks[i].valid = 1;
      pcache->sets[pi_bit].blocks[i].tag = ptag;
      pcache->sets[pi_bit].blocks[i].count = Count;
      pr++;
      return;
    } else {
      if (pcache->sets[pi_bit].blocks[i].valid == 1) {
	if (pcache->sets[pi_bit].blocks[i].tag == ptag){
	  /*
	    if (!strcmp(policy, "lru")){
	    Count++;
	    pcache->sets[pi_bit].blocks[i].count = Count;
	    return;
	    }
	  */
	  return;
	} else {
	  if (i == (assoc-1)){
	    Count++;
	    if (!strcmp(type,  "direct")){
	      pcache->sets[pi_bit].blocks[i].valid = 1;
	      pcache->sets[pi_bit].blocks[i].tag = ptag;
	      pcache->sets[pi_bit].blocks[i].count = Count;
	      pr++;
	      return;
	    } else {
	      //finds the mimimum 
	      int smallelem = pcache->sets[pi_bit].blocks[0].count;
	      int smallindex;
	      for (int j = 0; j < assoc; j++){
		if (pcache->sets[pi_bit].blocks[j].count <= smallelem){
		  smallelem = pcache->sets[pi_bit].blocks[j].count;
		  smallindex = j;
		}
	      }
	      pcache->sets[pi_bit].blocks[smallindex].valid = 1;
	      pcache->sets[pi_bit].blocks[smallindex].tag = ptag;
	      pcache->sets[pi_bit].blocks[smallindex].count = Count;
	      pr++;
	      return;
	    }
	  }
	}
      }
    }
  }
  return;
}


int main(int argc, char** argv){
  if (argc != 6){
    printf("Invalid Input\n");
    return 0;
  }
  
  int cache_size = atoi(argv[1]);
  char *type = argv[2];
  char *policy = argv[3];
  int b_size = atoi(argv[4]);
  int num_set;
  int assoc;
  
  if (!strcmp(type, "direct")){
    assoc = 1;
    num_set = cache_size / (assoc * b_size);
  } else if (!strcmp(type, "assoc")) {
    num_set = 1;
    assoc = cache_size / (num_set * b_size);
  } else {
    sscanf(type, "assoc:%d", &assoc);
    num_set = cache_size / (assoc * b_size);
  }
  
  //create the cache
  struct cache Cache;
  Cache.sets = (struct set*) malloc(sizeof(struct set) * num_set);
  for (int i = 0; i < num_set; i++){
    Cache.sets[i].blocks = (struct block*) malloc(sizeof(struct block) * assoc);
    for (int j = 0; j < assoc; j++){
      Cache.sets[i].blocks[j].valid = 0;
    }
  }
  //for prefetch 
  struct cache Pcache;
  Pcache.sets = (struct set*) malloc(sizeof(struct set) * num_set);
  for (int i = 0; i < num_set; i ++){
    Pcache.sets[i].blocks = (struct block*) malloc(sizeof(struct block) * assoc);
    for (int j = 0; j < assoc; j++){
      Pcache.sets[i].blocks[j].valid = 0;
    }
  }
  
  int b_bit = log2(b_size);
  int i_bit = log2(num_set);
  FILE * fp;
  unsigned long long int pc;
  unsigned long long int ad;
  unsigned long long int new_address;
  unsigned long long int tag_bits;
  unsigned long long int mask = ((pow(2, i_bit)) - 1);
  char c;
  int hit = 0;
  int miss = 0;
  int r = 0;
  int w = 0;
  unsigned long long int new_paddress;
  unsigned long long int ptag_bits;
  int phit = 0;
  int pmiss = 0;
  int pw = 0;
  FILE * fp2;
  unsigned long long int set_ibits;
  unsigned long long int pb_bit = log2(b_size);
  unsigned long long int pi_bit = log2(num_set);
  int setpi_bit;
  
  fp = fopen(argv[5], "r");
  if (fp == NULL) return 0;
  if ((!strcmp(policy, "fifo")) || (!strcmp(policy, "lru"))){
    while (fscanf(fp, "%llx: %c %llx\n", &pc, &c, &ad) == 3){
      new_address = (ad >> b_bit);
      set_ibits = new_address & mask;
      tag_bits = (ad >> (b_bit + i_bit));
      
      if (c == 'W'){
	if((simulatefifo(&Cache, tag_bits, set_ibits, assoc, policy, type) == 0)){
	  miss++;
	  r++;
	  w++;
	} else {
	  hit++;
	  w++;
	}
      } else if (c == 'R'){
	if ((simulatefifo(&Cache, tag_bits, set_ibits, assoc, policy, type) == 0)){
	  r++;
	  miss++;
	} else {
	  hit++;
	}
      }	
    }
    
    Count = 0;
    
    printf("no-prefetch\n");
    printf("Memory reads: %d\n", r);
    printf("Memory writes: %d\n", w);
    printf("Cache hits: %d\n", hit);
    printf("Cache misses: %d\n", miss);
    
    fp2 = fopen(argv[5], "r");
    if (fp2 == NULL) return 0;
    while (fscanf(fp2, "%llx: %c %llx\n", &pc, &c, &ad) == 3){
      new_address = (ad >> b_bit);
      set_ibits = new_address & mask;
      tag_bits = (ad >> (b_bit + i_bit));
      
      new_paddress = ad + b_size;
      setpi_bit = (new_paddress >> pb_bit) & mask;
      ptag_bits = (new_paddress >> (pb_bit + pi_bit));
      
      if (c == 'W'){
	if ((prefetchfifo(&Pcache, tag_bits, ptag_bits, set_ibits, assoc, setpi_bit, policy, type) == 0)){
	  pw++;
	  pr++;
	  pmiss++;
	} else {
	  pw++;
	  phit++;
	}
      } else if (c =='R'){
	if ((prefetchfifo(&Pcache, tag_bits, ptag_bits, set_ibits, assoc, setpi_bit, policy, type) == 0)){
	  pr++;
	  pmiss++;
	} else {
	  phit++;
	}
      }
    }
    
    printf("with-prefetch\n");
    printf("Memory reads: %d\n", pr);
    printf("Memory writes: %d\n", pw);
    printf("Cache hits: %d\n", phit);
    printf("Cache misses: %d\n", pmiss);
  }
  
  for (int i = 0; i < num_set; i++){
    free(Cache.sets[i].blocks);
  }
  
  free(Cache.sets);
  fclose(fp);
  fclose(fp2);
  
  for (int i = 0; i < num_set; i++){
    free(Pcache.sets[i].blocks);
  }
  
  free(Pcache.sets);
  return 0;
}
