#include<stdio.h>
#include<stdlib.h>
#define INFINITY 99999 

int vertices,edges;

struct vertex{
	int dist;			// Stores the distance from the source vertex
	int pos;			// holds the position of the vertex on the heap
	struct edge *list;	// Pointer to the first edge on the list
	struct edge *last;	// Pointer to the last edge on the list
};

struct edge{
	int srce;			// Source vertex of the edge
	int dest;			// Destination vertex of the edge
	int weight;			// Weight of the edge
	struct edge *next;	// Pointer to the next edge on the list
};

struct vertexOnHeap{
	int vertex;
	int dist;
};


/*	This function replaces the first element with the last element on the heap *
	and maintains the Minimum Heap property by perculating down till the leaf  */
void perculateDown(struct vertexOnHeap *vOH,struct vertex *vt,int size){
	int i,left,right,min;
	struct vertexOnHeap temp;
	for(i=0;i<size/2;i=min){
		left = 2*i + 1;
		right = 2*i + 2;
		if(right >= size){
			if(vOH[i].dist <= vOH[left].dist){
				min = i;
			}else{
				min = left;
			}
		}else{
			if(vOH[i].dist <= vOH[left].dist && vOH[i].dist <= vOH[right].dist){
				min = i;
			}else if(vOH[left].dist <= vOH[right].dist){
				min = left;
			}else{
				min = right;
			}
		}

		if(min == i){
			break;
		}else{
// swapping the struct vertex on the heap
			temp.vertex = vOH[i].vertex;
			temp.dist = vOH[i].dist;
			vOH[i].vertex = vOH[min].vertex;
			vOH[i].dist = vOH[min].dist;
			vOH[min].vertex = temp.vertex;
			vOH[min].dist = temp.dist;
			vt[vOH[min].vertex].pos = min;
			vt[vOH[i].vertex].pos = i;			
		}
	}
}

/*	This function maintains the Minimum heap property          *
 *  when a vertex is updated by perculating up till the root */
void preculateUp(struct vertexOnHeap *vOH,struct vertex *vt,int index){
	int ind=index,parent,min;
	struct vertexOnHeap temp;
	while(ind>0){
		parent = (ind-1)/2;
		if(vOH[parent].dist <= vOH[ind].dist){
			min = parent;
		}else{
			min = ind;
		}

		if(min == parent){
			break;
		}else{
			temp.vertex = vOH[ind].vertex;
			temp.dist = vOH[ind].dist;
			vOH[ind].vertex = vOH[parent].vertex;
			vOH[ind].dist = vOH[parent].dist;
			vOH[parent].vertex = temp.vertex;
			vOH[parent].dist = temp.dist;
			vt[vOH[parent].vertex].pos = parent;
			vt[vOH[ind].vertex].pos = ind;
		}
		ind = parent;
	}
}

//	Extracts the root from the Heap
int extractMin(struct vertexOnHeap *vOH,struct vertex *vt,int size){
	int min = vOH[0].vertex;
	struct vertexOnHeap temp;
	temp.vertex = vOH[0].vertex;
	temp.dist = vOH[0].dist;
	vOH[0].vertex = vOH[size-1].vertex;
	vOH[0].dist = vOH[size-1].dist;
	vOH[size-1].vertex = temp.vertex;
	vOH[size-1].dist = temp.dist;
	vt[vOH[size-1].vertex].pos = size-1;
	vt[vOH[0].vertex].pos = 0;
	size--;
	perculateDown(vOH,vt,size);
	return min;
}

//	This function implements the Dijskra's algo
void dijkstra(struct vertex *vt){
	int i,u,size;
	struct vertexOnHeap *vOH = malloc(vertices * sizeof(struct vertexOnHeap));
	struct edge *temp;
	for(i=0;i<vertices;i++){
		if(i==0){
			vOH[i].dist = 0;
		}else{
			vOH[i].dist = INFINITY;
		}
		vOH[i].vertex = i;
	}

	for(size=vertices;size>0;size--){
		u = extractMin(vOH,vt,size);
		temp = vt[u].list;
		while(temp != NULL){
			if(vt[temp->dest].dist > vt[u].dist + temp->weight){
				vt[temp->dest].dist = vt[u].dist + temp->weight;
				vOH[vt[temp->dest].pos].dist = vt[temp->dest].dist;
				preculateUp(vOH,vt,vt[temp->dest].pos);
			}
			temp = temp->next;
		}
	}
}

//	Adds an Edge to the list maintained by the Vertex
void addEdgeToList(int u,int v,int w,struct vertex *vt){
	struct edge *newEdge;
	newEdge = malloc(sizeof(struct edge));
	newEdge->srce = u;
	newEdge->dest = v;
	newEdge->weight = w;
	newEdge->next = NULL;

	if(vt[u].list == NULL && vt[u].last == NULL){
		vt[u].list = newEdge;
		vt[u].last = newEdge;
	}else{
		vt[u].last->next = newEdge;
		vt[u].last = newEdge;
	}
}

int main(){
	int i,u,v,w,mostDistant=-1;
	scanf("%d",&vertices);
	scanf("%d",&edges);

	struct vertex *vrtx = malloc(vertices * sizeof(struct vertex));
	for(i=0;i<vertices;i++){
		if(i==0){
			vrtx[i].dist = 0;
		}else{
			vrtx[i].dist = INFINITY;
		}
		vrtx[i].pos = i;
		vrtx[i].list = NULL;
		vrtx[i].last = NULL;
	}
	
	for(i=0;i<edges;i++){
		scanf("%d",&u);
		scanf("%d",&v);
		scanf("%d",&w);
		addEdgeToList(u,v,w,vrtx);
		addEdgeToList(v,u,w,vrtx);
	}

	dijkstra(vrtx);

	for(i=0;i<vertices;i++){
		if(vrtx[i].dist > mostDistant){
			mostDistant = vrtx[i].dist;
		}
	}
	printf("Distance from the most distant vertex: %d\n",mostDistant);
	free(vrtx);
	return 0;
}

