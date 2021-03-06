#include "Event/Event.h"

// LDMX
#include "Event/SimCalorimeterHit.h"
#include "Event/SimParticle.h"
#include "Event/SimTrackerHit.h"

// ROOT
#include "TClonesArray.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

// STL
#include <stdio.h>
#include <iostream>

int main(int, const char* argv[])  {

    std::cout << "Hello LDMX Event Model Test!" << std::endl;

    /*
     * Open ROOT file for writing.
     */
    TFile* rootFile = new TFile("ldmx_event_model_test.root", "RECREATE");
    TTree *tree = new TTree("LDMX_Event", "LDMX event tree");
    Event* event = new Event();
    tree->Branch("Event", "Event", &event, 32000, 3);

    /*
     * Create a dummy event.
     */
    EventHeader* eventHeader = event->header();
    eventHeader->setTimestamp(1473280918);
    eventHeader->setRun(1);
    eventHeader->setEventNumber(1000);

    SimCalorimeterHit* calHit = (SimCalorimeterHit*) event->addObject("EcalSimHits");
    calHit->setEdep(1.234);
    calHit->setId(11111111L);
    calHit->setPosition(10., 20., 5000.);

    SimTrackerHit* trackerHit = (SimTrackerHit*) event->addObject("RecoilSimHits");
    trackerHit->setEdep(2.345);
    trackerHit->setEndPosition(50., 40., 2000.);
    trackerHit->setStartPosition(60., 50., 2001.);
    trackerHit->setId(22222222L);
    trackerHit->setMomentum(1.0, 2.0, 3.0);
    trackerHit->setTime(42.);

    SimParticle* particle = (SimParticle*) event->addObject("SimParticles");
    particle->setEndPoint(60., 70., 2002.);
    particle->setEnergy(3.45);
    particle->setGenStatus(1);
    particle->setSimStatus(2);
    particle->setMass(85.);
    particle->setMomentum(80., 90., 2003.);
    particle->setPdg(11);
    particle->setTime(69.);
    particle->setVertex(90., 100., 2004.);

    std::cout << "EventHeader: run = " << event->header()->run() << "; eventNum = " << eventHeader->eventNumber() << "; timestamp = " << eventHeader->timestamp() << std::endl;
    std::cout << "Created event has " << event->collectionSize("EcalSimHits") << " EcalSimHits" << std::endl;
    std::cout << "Created event has " << event->collectionSize("RecoilSimHits") << " RecoilSimHits" << std::endl;
    std::cout << "Created event has " << event->collectionSize("SimParticles") << " SimParticles" << std::endl;

    /**
     * Fill the ROOT tree with event data.
     */
    std::cout << "Filling ROOT tree ..." << std::endl;
    tree->Fill();

    /**
     * Write out the ROOT file.
     */
    std::cout << "Writing ROOT file ..." << std::endl;
    rootFile->Write();

    /**
     * Read the ROOT file back in.
     */
    TFile *readFile = new TFile("ldmx_event_model_test.root");
    TTree *readTree = (TTree*) readFile->Get("LDMX_Event");
    Event* readEvent = new Event();
    TBranch *branch = readTree->GetBranch("Event");
    branch->SetAddress(&readEvent);

    std::cout << "Reading ROOT file ..." << std::endl;
    for(int entry = 0; entry < tree->GetEntries(); ++entry) {
        tree->GetEntry(entry);
        std::cout << "Read entry " << entry << " from file." << std::endl;
        std::cout << "Read EventHeader: run = " << event->header()->run() << "; eventNum = " << eventHeader->eventNumber() << "; timestamp = " << eventHeader->timestamp() << std::endl;
        std::cout << "Read event has " << event->collectionSize("EcalSimHits") << " EcalSimHits" << std::endl;
        TClonesArray* coll = event->collection("EcalSimHits");
        for (int i = 0; i < event->collectionSize("EcalSimHits"); i++) {
            coll[i].Print();
        }
        std::cout << "Read event has " << event->collectionSize("RecoilSimHits") << " RecoilSimHits" << std::endl;
        coll = event->collection("RecoilSimHits");
        for (int i = 0; i < event->collectionSize("RecoilSimHits"); i++) {
            coll[i].Print();
        }
        std::cout << "Read event has " << event->collectionSize("SimParticles") << " SimParticles" << std::endl;
        coll = event->collection("SimParticles");
        for (int i = 0; i < event->collectionSize("SimParticles"); i++) {
            coll[i].Print();
        }
        std::cout << std::endl;
    }

    /**
     * Delete the event.
     */
    delete event;

    std::cout << "Bye LDMX Event Model Test!" << std::endl;

    return 0;
}
