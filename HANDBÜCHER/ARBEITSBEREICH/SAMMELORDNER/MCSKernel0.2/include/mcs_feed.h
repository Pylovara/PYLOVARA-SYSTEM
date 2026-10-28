#ifndef MCS_FEED_H
#define MCS_FEED_H

// Lädt ein Protein in den flüchtigen Feed-Cache und das Register
void mcs_feed_laden(int id, const char* inhalt);

// Liest den aktuellen Status eines Feeds (bevorzugt aus Cache)
const char* mcs_feed_lesen(int id);

#endif
