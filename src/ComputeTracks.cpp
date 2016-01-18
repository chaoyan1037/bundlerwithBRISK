/* 
 *  Copyright (c) 2008-2010  Noah Snavely (snavely (at) cs.cornell.edu)
 *    and the University of Washington
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */

/* ComputeTracks.cpp */
/* Code for linking matches into tracks */

#include <queue>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "keys.h"

#include "BundlerApp.h"
#include "SifterUtil.h"

bool CompareFirst(const KeypointMatch &k1, const KeypointMatch &k2) {
    return (k1.m_idx1 < k2.m_idx1);
}

/* Compute a set of tracks that explain the matches */
#define LARGE_NUMBER 99999999
void BundlerApp::ComputeTracks(int new_image_start) 
{
    unsigned int num_images = GetNumImages();

    /* Clear all marks for new images */
    for (unsigned int i = 0; i < num_images; i++) {
        int num_nbrs = (int) m_matches.GetNumNeighbors(i);

        if (num_nbrs == 0)
            continue;

        int num_features = m_image_data[i].GetNumKeys();
        m_image_data[i].m_key_flags.resize(num_features);
    }

    int pt_idx = 0;
    std::vector<TrackData> tracks;

    for (unsigned int i = 0; i < num_images; i++) {
        MatchAdjList::iterator iter;
        for (iter = m_matches.Begin(i); iter != m_matches.End(i); iter++) {
            // MatchIndex idx = *iter;
            std::vector<KeypointMatch> &list = iter->m_match_list; // iter->second; // m_match_lists[idx];
            sort(list.begin(), list.end(), CompareFirst);
        }
    }

    bool *img_marked = new bool[num_images];
    memset(img_marked, 0, num_images * sizeof(bool));

    std::vector<int> touched;
    touched.reserve(num_images);

    for (unsigned int i = 0; i < num_images; i++) {
		int num_features = m_image_data[i].GetNumKeys(); // GetNumKeys(i);

        /* If this image has no neighbors, skip it */
        // std::list<unsigned int> &nbrs = m_matches.GetNeighbors(i);
        // int num_nbrs = (int) nbrs.size();
        int num_nbrs = (int) m_matches.GetNumNeighbors(i);

        if (num_nbrs == 0)
            continue;

	for (int j = 0; j < num_features; j++) {
	    ImageKeyVector features;
	    std::queue<ImageKey> features_queue;

        if (m_image_data[i].m_key_flags[j])
        continue; // already visited this feature

        /* Reset flags */
        int num_touched = touched.size();
        for (int k = 0; k < num_touched; k++)
            img_marked[touched[k]] = false;
        touched.clear();

        m_image_data[i].m_key_flags[j] = true;

	    features.push_back(ImageKey(i, j));
	    features_queue.push(ImageKey(i, j));

        img_marked[i] = true;
        touched.push_back(i);

	    int num_rounds = 0;
	    while (!features_queue.empty()) {
			num_rounds++;

			ImageKey feature = features_queue.front();
			features_queue.pop();
		
			int img1 = feature.first;
			int f1 = feature.second;
			KeypointMatch dummy;
			printf("computetracks 111 outter...\n");
			//printf("computetracks 149, get a feature pair from the quequ \\
			//		and try to find the match...\n");
			dummy.m_idx1 = f1;

			int start_idx;
			/* Limit new images to point only to other new images */
			if (img1 >= new_image_start) {
				start_idx = new_image_start;
			} else {
				start_idx = 0;
			}

            /* Check all adjacent images */
            MatchAdjList &nbrs = m_matches.GetNeighbors(img1);
            MatchAdjList::iterator iter;
            for (iter = nbrs.begin(); iter != nbrs.end(); iter++) {
                unsigned int k = iter->m_index; 

                if (img_marked[k])
                    continue;

				/* Skip non-matching images */
				// if (!ImagesMatch(img1, k))
                //     continue;

				MatchIndex base = GetMatchIndex(img1, k);

                std::vector<KeypointMatch> &list = 
                    m_matches.GetMatchList(base); // m_match_lists[base];

                /* Do a binary search for the feature */
                std::pair<std::vector<KeypointMatch>::iterator, 
                            std::vector<KeypointMatch>::iterator> p;
				printf("computetracks 144 inner...\n");
				//printf("computetracks 188, for each new match from the quequ, from more match from its neighbor...\n");
                p = equal_range(list.begin(), list.end(), 
                                dummy, CompareFirst);

                if (p.first == p.second)
                    continue;  /* not found */

                assert((p.first)->m_idx1 == f1);
                int idx2 = (p.first)->m_idx2;
			    
                /* Check if we visited this point already */
                // if (GetKey(k,idx2).m_extra >= 0)
                //     continue;
                assert(idx2 < m_image_data[k].GetNumKeys());

                if (m_image_data[k].m_key_flags[idx2])
                    continue;

                /* Mark and push the point */
                // GetKey(k,idx2).m_extra = pt_idx;
                m_image_data[k].m_key_flags[idx2] = true;
                features.push_back(ImageKey(k, idx2));
                features_queue.push(ImageKey(k, idx2));

                img_marked[k] = true;
                touched.push_back(k);
			}
		} /* while loop */

	    if (features.size() >= 2) {
			printf("Point with %d projections found\n", 
				   (int) features.size());
			fflush(stdout);

			tracks.push_back(TrackData(features));

			pt_idx++;
	    } 

		} /* for loop over features */
    } /* for loop over images */

    printf("[ComputeTracks] Found %d points\n", pt_idx);
    fflush(stdout);

    if (pt_idx != (int) tracks.size()) {
	printf("[ComputeTracks] Error: point count "
	       "inconsistent!\n");
	fflush(stdout);
    }

    /* Clear match lists */
    printf("[ComputeTracks] Clearing match lists...\n");
    fflush(stdout);

    RemoveAllMatches();

    /* Create the new consistent match lists */
    printf("[ComputeTracks] Creating consistent match lists...\n");
    fflush(stdout);

    int num_pts = pt_idx;

    for (int i = 0; i < num_pts; i++) {
		int num_features = (int) tracks[i].m_views.size();

        for (int j = 0; j < num_features; j++) {
            int img1 = tracks[i].m_views[j].first;
            int key1 = tracks[i].m_views[j].second;

            m_image_data[img1].m_visible_points.push_back(i);
            m_image_data[img1].m_visible_keys.push_back(key1);
        }        
    }

    /* Save the tracks */
    m_track_data = tracks;

    // SetMatchesFromTracks();

    printf("[ComputeTracks] Done!\n");
    fflush(stdout);
}
#undef LARGE_NUMBER
