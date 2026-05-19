#include <stdio.h>

#define INFINITY 9999
#define MAX 10

int cost[MAX][MAX], dist[MAX][MAX], next_hop[MAX][MAX];
int nodes;

// Initialize routing tables
void initialize() {
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            dist[i][j] = cost[i][j];
            next_hop[i][j] = j;
        }
    }
}

// Update routes using Distance Vector Routing algorithm
void updateRoutes() {
    int updated;

    do {
        updated = 0;

        for (int i = 0; i < nodes; i++) {
            for (int j = 0; j < nodes; j++) {
                for (int k = 0; k < nodes; k++) {

                    // Check for valid paths before updating
                    if (dist[i][k] != INFINITY &&
                        dist[k][j] != INFINITY &&
                        dist[i][j] > dist[i][k] + dist[k][j]) {

                        dist[i][j] = dist[i][k] + dist[k][j];
                        next_hop[i][j] = next_hop[i][k];

                        updated = 1;
                    }
                }
            }
        }

    } while (updated);
}

// Display routing tables
void display() {
    for (int i = 0; i < nodes; i++) {

        printf("\nRouter %d Routing Table:\n", i + 1);
        printf("Destination\tCost\tNext Hop\n");

        for (int j = 0; j < nodes; j++) {

            if (dist[i][j] == INFINITY)
                printf("%d\t\tINF\t-\n", j + 1);
            else
                printf("%d\t\t%d\t%d\n",
                       j + 1,
                       dist[i][j],
                       next_hop[i][j] + 1);
        }
    }
}

int main() {

    printf("Enter the number of routers: ");
    scanf("%d", &nodes);

    printf("Enter the cost matrix (9999 for no direct link):\n");

    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {

            scanf("%d", &cost[i][j]);

            if (i == j)
                cost[i][j] = 0;
        }
    }

    initialize();

    updateRoutes();

    display();

    return 0;
}