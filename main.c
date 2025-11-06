#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void returnJourney(int stack[], int top, int graph[25][25], int n, char areas[][30], int timeSlot);


// 1. Display functions
void displayMainMenu() {
    printf("\n============================\n");
    printf("   TRANSPORT ROUTE SCHEDULER    \n");
    printf("==============================\n");
    printf("This program helps you find the shortest route,\n");
    printf("estimate travel time, judge your return journey and view transport options!\n");

    printf("\nPlease press Enter to continue");
    while (getchar() != '\n');  // Wait until Enter is pressed
}

int displayTransportMenu() {
    int choice;
    printf("\nSelect Mode of Transport:\n");
    printf("1. Bus \n");
    printf("2. Car \n");
    printf("3. Bike/Scooter \n");
    printf("4. Exit \n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    while (choice < 1 || choice > 4) {
        printf("Invalid choice. Please enter a number between 1 and 4: ");
        scanf("%d", &choice);
    }
    return choice;
}

int displayAreaMenu(char areas[][30], int n, char *prompt) {
    int choice;
    printf("\n%s\n", prompt);
    for (int i = 0; i < n; i++) {
        printf("%d. %s\n", i + 1, areas[i]);
    }
    printf("Enter your choice (1-%d): ", n);
    scanf("%d", &choice);
    while (choice < 1 || choice > n) {
        printf("Invalid choice. Please enter a number between 1 and %d: ", n);
        scanf("%d", &choice);
    }
    return choice - 1;
}

// 2. Graph and Dijkstra functions
void buildGraph(int graph[25][25], int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            graph[i][j] = 0;

    // Baner connections
    graph[7][8] = graph[8][7] = 2;
    graph[7][6] = graph[6][7] = 4;
    graph[7][9] = graph[9][7] = 6;

    // Balewadi connections
    graph[8][19] = graph[19][8] = 4;

    // Wakad connections
    graph[19][18] = graph[18][19] = 3;
    graph[19][17] = graph[17][19] = 4;

    // Pimple Nilakh connections
    graph[17][16] = graph[16][17] = 2;
    graph[17][6] = graph[6][17] = 3;

    // Pimple Saudagar connections
    graph[16][20] = graph[20][16] = 6;
    graph[16][14] = graph[14][16] = 3;

    // Aundh connections
    graph[6][2] = graph[2][6] = 7;

    // Shivajinagar connections
    graph[2][3] = graph[3][2] = 4;
    graph[2][5] = graph[5][2] = 6;
    graph[2][9] = graph[9][2] = 8;

    // FC Road connections
    graph[3][1] = graph[1][3] = 1;
    graph[3][0] = graph[0][3] = 7;

    // Bund Garden connections
    graph[5][11] = graph[11][5] = 4;
    graph[5][15] = graph[15][5] = 4;

    // Kalyani Nagar connections
    graph[11][14] = graph[14][11] = 3;
    graph[11][15] = graph[15][11] = 2;
    graph[11][12] = graph[12][11] = 4;

    // Wadgaonsheri connections
    graph[14][12] = graph[12][14] = 4;
    graph[14][13] = graph[13][14] = 6;

    // Kharadi connections
    graph[13][22] = graph[22][13] = 6;

    // Hadapsar connections
    graph[22][0] = graph[0][22] = 5;

    // Camp connections
    graph[0][4] = graph[4][0] = 6;
    graph[0][21] = graph[21][0] = 6;

    // MG Road connections
    graph[4][24] = graph[24][4] = 5;

    // Kondhwa connections
    graph[24][23] = graph[23][24] = 6;

    // Bibwewadi connections
    graph[23][0] = graph[0][23] = 6;
}

int minDistance(int dist[], int visited[], int n) {
    int min = INT_MAX;  // (like around infity is the distance right now, any other real distance will always be less than that)
    int min_index = -1; // (index of node that has minimum distance )
    for (int v = 0; v < n; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

int dijkstra(int graph[25][25], int n, int start, int end, int parent[], char areas[][30]) {
    int dist[25], visited[25];
    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;   //distance to all the nodes is infinity initially
        visited[i] = 0;    //no nodes are visited yet
        parent[i] = -1;    //we dont know any predessecors yet
    }

    dist[start] = 0;

    for (int count = 0; count < n - 1; count++) {
        int u = minDistance(dist, visited, n);
        if (u == -1) break; //this happens when the node is not connected to any other places 
        visited[u] = 1;

        for (int v = 0; v < n; v++) { //this loop will look at all the other nodes 
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) { //if not visited and there is a connection and distance to u is not infinityand the new distance is less than the old distance
                dist[v] = dist[u] + graph[u][v]; //distance now shorter and updated 
                parent[v] = u; //now a new predessesor
            }
        }
    }

    printf("\nShortest distance from %s to %s = %d km\n", areas[start], areas[end], dist[end]);
    return dist[end];
}

void displayShortestPath(int parent[], int end, char areas[][30], int stack[], int *top) {
    int path[25], count = 0;
    for (int v = end; v != -1; v = parent[v]) {
        path[count++] = v;
    }

    printf(" Path: ");
    for (int i = count - 1; i >= 0; i--) {    //starting from the end and going backwards till we can't find a parent 
        printf("%s", areas[path[i]]);
        stack[++(*top)] = path[i]; //storing it the stack so we can find out return journey
        if (i != 0) printf(" -> ");
    }
    printf("\n");
}

// 3. Time and transport functions
int getTimeSlot() {
    char ans;
    int choice;
    time_t t;   //hold time as secs
    struct tm *currentTime;

    printf("\nAre you travelling right now? (y/n): ");
    scanf(" %c", &ans);
    ans = tolower(ans);

    if (ans == 'y') {
        time(&t);                 //gets local time 
        currentTime = localtime(&t); //converts that to local
        int hour = currentTime->tm_hour;  //24 hr clock

        if (hour >= 6 && hour < 11) choice = 1;  //morn
        else if (hour >= 11 && hour < 16) choice = 2; //afternoon
        else if (hour >= 16 && hour < 21) choice = 3; //eve
        else choice = 4;   //night

        printf("\nDetected current time: %02d:%02d\n", hour, currentTime->tm_min);
    } else {
        printf("\nSelect your time of travel:\n");
        printf("1. Morning (6 AM - 11 AM)\n");
        printf("2. Afternoon (11 AM - 4 PM)\n");
        printf("3. Evening (4 PM - 9 PM)\n");
        printf("4. Night (9 PM - 6 AM)\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
    }
    return choice;
}

float calculateTime(float distance, int mode, int timeSlot) {
    float baseSpeed;
    float trafficFactor;

    switch (mode) {
        case 1: baseSpeed = 30; break;
        case 2: baseSpeed = 40; break;
        case 3: baseSpeed = 25; break;
        default: return -1;
    }

    switch (timeSlot) {
        case 1: trafficFactor = 0.8; break;
        case 2: trafficFactor = 1.0; break;
        case 3: trafficFactor = 0.6; break;
        case 4: trafficFactor = 1.3; break;
        default: return -1;
    }

    float effectiveSpeed = baseSpeed * trafficFactor;
    float timeHours = distance / effectiveSpeed;
    float timeMinutes = timeHours * 60;

    //Calculate ideal (no traffic) time
    float idealTime = (distance / baseSpeed) * 60;

    printf("\nEstimated travel time (with traffic): %.1f minutes", timeMinutes);
    printf("\nIdeal time (no traffic): %.1f minutes\n", idealTime);

    return timeMinutes;
}

// 3.5. Bus options function (with next bus ETA)
void displayBusOptions(int start, int end, char areas[][30], int timeSlot) {
    FILE *fp = fopen("bus_data.txt", "r");
    if (fp == NULL) {
        printf("\n Could not open bus_data.txt file.\n");
        return;
    }

    char line[256];
    int found = 0;

    printf("\n Available Bus Routes from %s to %s:\n", areas[start], areas[end]);
    printf("--------------------------------------------------\n");

    // Get current time
    time_t rawtime;
    struct tm *current;
    time(&rawtime);
    current = localtime(&rawtime);

    int currentHour = current->tm_hour;
    int currentMin = current->tm_min;
    int totalMinsNow = currentHour * 60 + currentMin;

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n') continue;

        char routeNo[10], from[30], to[30], stops[200];
        int freq, approxTime;

        if (sscanf(line, "%[^;];%[^;];%[^;];%[^;];%d;%d",
                   routeNo, from, to, stops, &freq, &approxTime) != 6)
            continue;

        // Convert all to lowercase for flexible comparisons
        char fromLower[30], toLower[30], stopsLower[200], startLower[30], endLower[30];
        strcpy(fromLower, from); strcpy(toLower, to); strcpy(stopsLower, stops);
        strcpy(startLower, areas[start]); strcpy(endLower, areas[end]);

        for (int i = 0; fromLower[i]; i++) fromLower[i] = tolower(fromLower[i]);
        for (int i = 0; toLower[i]; i++) toLower[i] = tolower(toLower[i]);
        for (int i = 0; stopsLower[i]; i++) stopsLower[i] = tolower(stopsLower[i]);
        for (int i = 0; startLower[i]; i++) startLower[i] = tolower(startLower[i]);
        for (int i = 0; endLower[i]; i++) endLower[i] = tolower(endLower[i]);

        int match = 0;

        // ✅ Direct, reverse, or via routes
        if (strcmp(fromLower, startLower) == 0 && strcmp(toLower, endLower) == 0) //direct route
            match = 1;
        else if (strcmp(toLower, startLower) == 0 && strcmp(fromLower, endLower) == 0) //reverse route
            match = 1;
        else if (strstr(stopsLower, startLower) && strstr(stopsLower, endLower)) //via route
            match = 1;
        else if (strcmp(fromLower, startLower) == 0 && strstr(stopsLower, endLower)) //start matches, end is via stops
            match = 1;
        else if (strcmp(toLower, endLower) == 0 && strstr(stopsLower, startLower)) //reverse start matches, end is via stops
            match = 1;

        if (match) {
            found = 1;

            // Adjust frequency based on time of day (traffic)
            int adjustedFreq = freq;
            if (timeSlot == 1) adjustedFreq += 2;   // Morning mild
            else if (timeSlot == 2) adjustedFreq += 5;  // Afternoon heavy
            else if (timeSlot == 3) adjustedFreq += 8;  // Evening peak
            else adjustedFreq -= 2;                    // Night — faster

            // Calculate next bus ETA
            int nextBusMins = adjustedFreq - (totalMinsNow % adjustedFreq);  // (totalMinsNow % adjustedFreq) → mins since last bus; subtract from adjustedFreq → mins until next bus

            if (nextBusMins == adjustedFreq) nextBusMins = 0; // arriving now

            int etaTotalMins = totalMinsNow + nextBusMins;
            int etaHour = (etaTotalMins / 60) % 24;
            int etaMin = etaTotalMins % 60;

            printf("Route %s: %s → %s", routeNo, from, to);
            if (strcmp(stops, "—") != 0 && strlen(stops) > 0)
                printf(" via %s", stops);

            printf("\n  Frequency: Every %d minutes", adjustedFreq);
            printf("\n  Approx. Travel Time: %d minutes", approxTime);
            printf("\n  Next Bus: In %d minutes (arrives at %02d:%02d)",
                   nextBusMins, etaHour, etaMin);
            printf("\n  (Adjusted for current traffic conditions)\n");
            printf("--------------------------------------------------\n");
        }
    }

    if (!found)
        printf("⚠️  No bus routes (direct, reverse, or via stops) found for this selection.\n");

    fclose(fp);
}

// 4. Return Journey Function
void returnJourney(int stack[], int top, int graph[25][25], int n, char areas[][30], int timeSlot) {
    char choice;
    printf("\nWould you like to calculate your return journey? (1/Yes = y, 2/No = n): ");
    scanf(" %c", &choice);
    choice = tolower(choice);

    if (choice != '1' && choice != 'y') {
        printf("\nThank you for using the Transport Route Scheduler!\n");
        return;
    }

    printf("\n==============================\n");
    printf("        RETURN JOURNEY        \n");
    printf("==============================\n");

    // Reverse the path stack
    int reversedStack[25];
    int reversedTop = -1;
    for (int i = top; i >= 0; i--)
        reversedStack[++reversedTop] = stack[i];

    // Display return path
    printf("\nReturn Route:\n");
    for (int i = 0; i <= reversedTop; i++) {
        printf("%s", areas[reversedStack[i]]);  //printing the foward stack in reverse order 
        if (i != reversedTop) printf(" -> ");
    }
    printf("\n");

    // Calculate total return distance
    int totalDistance = 0;
    for (int i = 0; i < reversedTop; i++) { 
        int from = reversedStack[i];
        int to = reversedStack[i + 1];
        if (graph[from][to] > 0)
            totalDistance += graph[from][to];
    }
    printf("\nTotal Return Distance: %d km\n", totalDistance);

    // Choose mode
    char ans;
    printf("\nDo you want to take a different mode of transport for the return journey? (y/n): ");
    scanf(" %c", &ans);
    ans = tolower(ans);

    int returnMode;
    if (ans == 'y') {
        printf("\nSelect Return Mode of Transport:\n");
        printf("1. Bus\n2. Car\n3. Bike/Scooter\n");
        printf("Enter your choice: ");
        scanf("%d", &returnMode);
        while (returnMode < 1 || returnMode > 3) {
            printf("Invalid choice. Enter 1, 2, or 3: ");
            scanf("%d", &returnMode);
        }
    } else {
        printf("\nUsing the same mode as your onward journey.\n");
        returnMode = 1;
    }

    // Choose time slot
    printf("\nAre you planning on returning in the same time slot as your onward journey? (y/n): ");
    char reuseTime;
    scanf(" %c", &reuseTime);
    reuseTime = tolower(reuseTime);

    int returnTimeSlot = (reuseTime == 'y') ? timeSlot : getTimeSlot();

    // Calculate time
    float returnTime = calculateTime((float)totalDistance, returnMode, returnTimeSlot);
    printf("\nEstimated Return Travel Time: %.1f minutes\n", returnTime);

    // Auto show bus routes
    if (returnMode == 1) {
        int start = reversedStack[0];
        int end = reversedStack[reversedTop];
        printf("\nWould you like to view available bus routes for your return journey? (y/n): ");
        char showBus;
        scanf(" %c", &showBus);
        if (tolower(showBus) == 'y')
            displayBusOptions(start, end, areas, returnTimeSlot);
    }

    printf("\n==============================\n");
    printf("      RETURN JOURNEY END      \n");
    printf("==============================\n");
}


// 5. Main Function
int main() {
    int n = 25;
    char areas[25][30] = {
        "Camp", "Deccan Gymkhana", "Shivajinagar", "FC Road", "MG Road",
        "Bund Garden", "Aundh", "Baner", "Balewadi", "Pashan", "Sangvi",
        "Kalyani Nagar", "Viman Nagar", "Kharadi", "Wadgaonsheri",
        "Koregaon Park", "Pimple Saudagar", "Pimple Nilakh", "Hinjewadi", "Wakad",
        "Chinchwad", "Wanowrie", "Hadapsar", "Bibwewadi", "Kondhwa"
    };

    int start, end, mode;
    displayMainMenu();

    mode = displayTransportMenu();
    if (mode == 4) {
        printf("Exiting program. Goodbye!\n");
        return 0;
    }

    start = displayAreaMenu(areas, n, "Select your starting area:");
    end = displayAreaMenu(areas, n, "Select your destination area:");

    printf("\nYou have selected:\nStart: %s\nDestination: %s\nMode: %d\n", areas[start], areas[end], mode);

    int graph[25][25], parent[25], stack[25], top = -1;
    buildGraph(graph, n);

    int distance = dijkstra(graph, n, start, end, parent, areas);
    displayShortestPath(parent, end, areas, stack, &top);

    int timeSlot = getTimeSlot();
    calculateTime((float)distance, mode, timeSlot);

    char seeBus;
    printf("\nDo you want to see available bus routes for this journey? (y/n): ");
    scanf(" %c", &seeBus);
    if (tolower(seeBus) == 'y') {
         displayBusOptions(start, end, areas, timeSlot);
    }

    returnJourney(stack, top, graph, n, areas, timeSlot);

    return 0;
}
