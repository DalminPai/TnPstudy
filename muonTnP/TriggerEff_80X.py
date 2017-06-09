import FWCore.ParameterSet.Config as cms
### USAGE:
###    cmsRun TriggerEff.py <scenario> [ <id> [ <binning1> ... <binningN> ] ]
###    ex> cmsRun TriggerEff.py mc IsoMu20_from_Tight2012
### scenarios:
###   - data_all (default)  
###   - signal_mc

Type = "Run2016G" ###

Ntuple_Data = "/data1/kplee/TagProbe/MuonPOG_80X/v20160929_1st_Extend_dPhiStudy/Run2016G/Run2016G_skimmed_TagCondition.root" ###
Ntuple_MC = ""


print "+" * 100;
print "Data Location: " + Ntuple_Data
print "MC location: " +Ntuple_MC
print "+" * 100;

import sys
args = sys.argv[1:]
if (sys.argv[0] == "cmsRun"): args =sys.argv[2:]
scenario = "data_all" ### default
if len(args) > 0: scenario = args[0]
print "Will run scenario ", scenario 

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

Template = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
    NumCPU = cms.uint32(1),
    SaveWorkspace = cms.bool(False),

    Variables = cms.PSet(
        mass = cms.vstring("Tag-muon Mass", "70", "130", "GeV/c^{2}"),
        pt = cms.vstring("muon p_{T}", "0", "1000", "GeV/c"),
        phi = cms.vstring("muon #phi", "-3.14", "3.14", ""),
        tag_pt = cms.vstring("tag muon p_{T}", "0", "1000", "GeV/c"),
        eta    = cms.vstring("muon #eta", "-2.5", "2.5", ""),
        abseta = cms.vstring("muon |#eta|", "0", "2.5", ""),
        tag_abseta = cms.vstring("tag muon |#eta|", "0", "2.5", ""),
        tag_nVertices = cms.vstring("Number of vertices", "0", "999", ""),
        combRelIsoPF04dBeta = cms.vstring("pf relative isolation", "0", "999", ""),
        tkIso = cms.vstring("Tracker Isolation", "0", "999", ""),
        relTkIso = cms.vstring("Corrected Tracker Isolation", "0", "999", ""), ###
	
        SIP = cms.vstring("Number of vertices", "0", "999", ""),
        run = cms.vstring("Number of vertices", "-999", "999999", ""),

        l1pt = cms.vstring("L1 p_{T}", "0", "1000", "GeV/C"),
        l1q = cms.vstring("L1 Quality", "0", "999", ""),
    ),

    Categories = cms.PSet(
        Glb   = cms.vstring("Global", "dummy[pass=1,fail=0]"),
        PF    = cms.vstring("PF Muon", "dummy[pass=1,fail=0]"),
        TM    = cms.vstring("Tracker Muon", "dummy[pass=1,fail=0]"),
        mvaIsoCut = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        mcTrue = cms.vstring("MC true", "dummy[true=1,false=0]"),
        Tight2012 = cms.vstring("Tight2012", "dummy[pass=1,fail=0]"),
        HighPt = cms.vstring("HighPtMuonID", "dummy[pass=1, fail=0]"),
        tag_IsoMu20 = cms.vstring("tag_IsoMu20", "dummy[pass=1,fail=0]"),
        tag_IsoMu24_eta2p1 = cms.vstring("tag_IsoMu24_eta2p1", "dummy[pass=1,fail=0]"),

        IsoMu20 = cms.vstring("IsoMu20", "dummy[pass=1,fail=0]"),
        IsoTkMu20 = cms.vstring("IsoTkMu20", "dummy[pass=1,fail=0]"),
        Mu45_eta2p1 = cms.vstring("Mu45_eta2p1", "dummy[pass=1,fail=0]"),
        Mu50 = cms.vstring("Mu50", "dummy[pass=1,fail=0]"),
        
        L1sMu16 = cms.vstring("L1sMu16", "dummy[pass=1,fail=0]"),
        Mu20 = cms.vstring("Mu20", "dummy[pass=1,fail=0]"),
        tag_Mu20 = cms.vstring("tag_Mu20", "dummy[pass=1,fail=0]"),
        L2fL1sMu16L1f0L2Filtered10Q = cms.vstring("L2fL1sMu16L1f0L2Filtered10Q", "dummy[pass=1,fail=0]"),

        tag_IsoMu24 = cms.vstring("tag_IsoMu24", "dummy[pass=1,fail=0]"), ###
        IsoMu24 = cms.vstring("IsoMu24", "dummy[pass=1,fail=0]"), ###
        IsoTkMu24 = cms.vstring("IsoTkMu24", "dummy[pass=1,fail=0]"), ###
    ),

    Expressions = cms.PSet(
        Exp_IsoMu20_OR_IsoTkMu20 = cms.vstring("Exp_IsoMu20_OR_IsoTkMu20", "IsoMu20==1 || IsoTkMu20==1", "IsoMu20", "IsoTkMu20"),
        Exp_L1SingleMu16 = cms.vstring("Exp_L1SingleMu16", "l1pt >= 16 && l1q >= 5", "l1pt", "l1q"),
        Exp_Tag_IsoMu20_OR_IsoMu24_eta2p1 = cms.vstring("Exp_Tag_IsoMu20_OR_IsoMu24_eta2p1", "tag_IsoMu20 || tag_IsoMu24_eta2p1", "tag_IsoMu20", "tag_IsoMu24_eta2p1"),
        Exp_IsoMu24_OR_IsoTkMu24 = cms.vstring("Exp_IsoMu24_OR_IsoTkMu24", "IsoMu24==1 || IsoTkMu24==1", "IsoMu24", "IsoTkMu24"), ###
    ),

    Cuts = cms.PSet(
                # combRelIsoPF04dBeta_015 = cms.vstring("combRelIsoPF04dBeta_015", "combRelIsoPF04dBeta", "0.15"),
                # combRelIsoPF04dBeta_025 = cms.vstring("combRelIsoPF04dBeta_015", "combRelIsoPF04dBeta", "0.25"),
                IsoMu20_OR_IsoTkMu20 = cms.vstring("IsoMu20_OR_IsoTkMu20", "Exp_IsoMu20_OR_IsoTkMu20", "0.5"),
                L1SingleMu16 = cms.vstring("L1SingleMu16", "Exp_L1SingleMu16", "0.5"),
                Tag_IsoMu20_OR_IsoMu24_eta2p1 = cms.vstring("Tag_IsoMu20_OR_IsoMu24_eta2p1", "Exp_Tag_IsoMu20_OR_IsoMu24_eta2p1", "0.5"),
                IsoMu24_OR_IsoTkMu24 = cms.vstring("IsoMu24_OR_IsoTkMu24", "Exp_IsoMu24_OR_IsoTkMu24", "0.5"), ###
    ),

    PDFs = cms.PSet(
        voigtPlusExpo = cms.vstring(
            "Voigtian::signal(mass, mean[90,80,100], width[2.495], sigma[3,1,20])",
            "Exponential::backgroundPass(mass, lp[0,-5,5])",
            "Exponential::backgroundFail(mass, lf[0,-5,5])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        vpvPlusExpo = cms.vstring(
            "Voigtian::signal1(mass, mean1[90,80,100], width[2.495], sigma1[2,1,3])",
            "Voigtian::signal2(mass, mean2[90,80,100], width,        sigma2[4,2,10])",
            "SUM::signal(vFrac[0.8,0,1]*signal1, signal2)",
            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        # -- PDF Sets for the case of failing on error calculation by MINOS -- #
        vpvPlusExpo2 = cms.vstring(
            "Voigtian::signal1(mass, mean1[91,86,96], width[2.495], sigma1[2,1,5])",
            "Voigtian::signal2(mass, mean2[91,81,101], width,        sigma2[6,3,10])",
            "SUM::signal(vFrac[0.8,0,1]*signal1, signal2)",
            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        vpvPlusExpoMin70 = cms.vstring(
            "Voigtian::signal1(mass, mean1[90,80,100], width[2.495], sigma1[2,1,3])",
            "Voigtian::signal2(mass, mean2[90,80,100], width,        sigma2[4,3,10])",
            "SUM::signal(vFrac[0.8,0.5,1]*signal1, signal2)",
            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0.7,1]",
            "signalFractionInPassing[0.9]"
        )
    ),

    binnedFit = cms.bool(True),
    binsForFit = cms.uint32(40),
    saveDistributionsPlot = cms.bool(False),

    Efficiencies = cms.PSet(), # will be filled later
)

PassingProbe = ""
if "_from_" in args[1]:
    PassingProbe = args[1].split("_from_")[0]
else:
    PassingProbe = args[1]
print "PassingProbe: " + PassingProbe

PtMin = 9999
if "Mu20" in PassingProbe: PtMin = 22
elif "Mu45" in PassingProbe: PtMin = 47
elif "Mu50" in PassingProbe: PtMin = 52
elif "L1SingleMu16" in PassingProbe: PtMin = 18
elif "Mu24" in PassingProbe: PtMin = 26 ###

EtaMax = 2.4
AbsEtaBin = cms.vdouble(0.0, 0.9, 1.2, 2.1, 2.4)
if "eta2p1" in PassingProbe: 
    EtaMax = 2.1
    AbsEtaBin = cms.vdouble()


PT_ETA_BINS = cms.PSet(
                        pt     = cms.vdouble( 0, 9999 ), # -- Will be set later -- #
                        abseta = cms.vdouble(0.0, 0.9, 1.2, 2.1, 2.4),

                        tag_IsoMu24 = cms.vstring("pass"), ###
                        tag_pt =  cms.vdouble(26,9999) ###
)
if "Mu20" in PassingProbe: PT_ETA_BINS.pt = cms.vdouble( 22, 25, 30, 40, 50, 60, 120 ) 
elif "Mu45" in PassingProbe: PT_ETA_BINS.pt = cms.vdouble( 47, 50, 60, 120 )
elif "Mu50" in PassingProbe: PT_ETA_BINS.pt = cms.vdouble( 52, 60, 120 )

if EtaMax == 2.1: PT_ETA_BINS.abseta = cms.vdouble(0.0, 0.9, 1.2, 2.1)


PT_BINS = cms.PSet(
                        pt     = cms.vdouble( 0, 9999 ), #Will be set later

                        abseta = cms.vdouble(0.0, EtaMax),
                        tag_IsoMu24 = cms.vstring("pass"), ###
                        tag_pt =  cms.vdouble(26,9999), ###
                        tag_abseta =  cms.vdouble(0, 2.4),
)
if "Mu20" in PassingProbe: PT_BINS.pt = cms.vdouble( 0, 10, 15, 18, 20, 22, 25, 30, 40, 50, 60, 80, 120 ) 
elif "Mu24" in PassingProbe: PT_BINS.pt = cms.vdouble( 0, 10, 15, 19, 22, 24, 26, 29, 40, 50, 60, 80, 120, 160, 200, 250, 300, 400, 500, 600, 800, 1000, 1500 ) ###
elif "Mu45" in PassingProbe: PT_BINS.pt = cms.vdouble( 0, 10, 15, 20, 25, 30, 40, 43, 45, 47, 50, 60, 80, 120 )
elif "Mu50" in PassingProbe: PT_BINS.pt = cms.vdouble( 0, 10, 15, 20, 25, 30, 40, 45, 48, 50, 52, 55, 60, 80, 120 )
elif "L1SingleMu16" in PassingProbe: PT_BINS.pt = cms.vdouble( 0, 10, 14, 16, 18, 20, 25, 30, 40, 50, 60, 80, 120 )

ETA_BINS = cms.PSet(
  			eta = cms.vdouble(-2.4, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.4),

                        pt     = cms.vdouble( PtMin, 9999 ),
                        tag_IsoMu24 = cms.vstring("pass"), ###
                        tag_pt =  cms.vdouble(26,9999), ###
                        tag_abseta =  cms.vdouble(0, 2.4),
)
if EtaMax == 2.1: ETA_BINS.eta = cms.vdouble(-2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1)

phi = 3.141592
degree15 = phi / 12;
PHI_BINS = cms.PSet(
	             	  phi     = cms.vdouble( (-1)*degree15*12, (-1)*degree15*11, (-1)*degree15*9, (-1)*degree15*7, (-1)*degree15*5, (-1)*degree15*3, (-1)*degree15*1, degree15*1, degree15*3, degree15*5, degree15*7, degree15*9, degree15*11, degree15*12),
                    
                  	  pt     = cms.vdouble( PtMin, 9999 ),
           	          abseta = cms.vdouble(0.0, EtaMax),
               	          tag_IsoMu24 = cms.vstring("pass"), ###
               	          tag_pt =  cms.vdouble(26,9999), ###
               	          tag_abseta =  cms.vdouble(0, 2.4),
)

VTX_BINS  = cms.PSet(
			tag_nVertices = cms.vdouble(2.5, 4.5, 6.5, 8.5, 10.5, 12.5, 14.5, 16.5, 18.5, 20.5, 22.5, 24.5, 26.5),

 	  	        pt     = cms.vdouble(  PtMin, 9999 ),
  			abseta = cms.vdouble(  0.0, EtaMax),
 			tag_IsoMu24 = cms.vstring("pass"), ###
  			tag_pt =  cms.vdouble(26,9999), ###
)

process.TnP_MuonID = Template.clone(
    InputFileNames = cms.vstring(),
    InputTreeName = cms.string("fitter_tree"),
    InputDirectoryName = cms.string("tpTree"),
    OutputFileName = cms.string("TnP_MuonTrigger_%s.root" % scenario),
    Efficiencies = cms.PSet(),
)

#Add the variables for PU reweighting
if "_weight" in scenario:
    process.TnP_MuonID.WeightVariable = cms.string("weight")
    process.TnP_MuonID.Variables.weight = cms.vstring("weight","-10","10","")

if scenario=="data_25ns":
    process.TnP_MuonID.InputFileNames = cms.vstring(
                                                    # put here the trees corresponding to data
                                                    "file:"+Ntuple_Data
                                                    )

if "mc_weight" in scenario:
    process.TnP_MuonID.InputFileNames = cms.vstring(
                                                    # put here the trees corresponding to mc
                                                    "file:"+Ntuple_MC
                                                    )

#IDS = [ "IsoMu20","Mu20","L2fL1sMu16L1f0L2Filtered10Q","IsoTkMu20","L1sMu16"]
IDS = [args[1]] #here the id is taken from the arguments provided to cmsRun 
# ALLBINS = [ ("pt",PT_BINS), ("eta",ETA_BINS), ("vtx",VTX_BINS), ("pteta",PT_ETA_BINS) ]
# ALLBINS = [("pt",PT_BINS),("eta",ETA_BINS),("vtx",VTX_BINS)]
ALLBINS = [("pt",PT_BINS)]
# ALLBINS = [("run", RUN_BINS)]

if len(args) > 1 and args[1] not in IDS: IDS += [ args[1] ]
for ID in IDS:
    print "now doing ",ID
    if len(args) > 1 and ID != args[1]: continue
    for X,B in ALLBINS:
        if len(args) > 2 and X not in args[2:]: continue
        #Add the information about ID and binning into the outputFileName
        module = process.TnP_MuonID.clone(OutputFileName = cms.string("TnP_MuonTrigger_%s_%s_%s.root" % (scenario, ID, X)))
        
        shape = "vpvPlusExpo"
        
        #DEN: Binning
        DEN = B.clone(); num = ID; numstate = "pass"

        if "_from_" in ID:
            parts = ID.split("_from_")
            num = parts[0]
            # add Additional ID conditions to the binning ... 
            # ex> cmsRun TriggerEff.py mc IsoMu20_and_Tight2012_from_SIP4_and_PFIso25 => SIP4 and PFIso25 info. is added to the binning definition
            for D in parts[1].split("_and_"):
                if D == "SIP4":      DEN.SIP = cms.vdouble(0,4.0)
                elif D == "PFIso25": DEN.pfCombRelIso04EACorr  = cms.vdouble(0,0.25)
                elif D == "PFIso40": DEN.pfCombRelIso04EACorr  = cms.vdouble(0,0.40)
                # elif D == "dBeta": DEN.combRelIsoPF04dBeta = cms.vdouble(0, 0.12)
                elif D == "dBeta_015": DEN.combRelIsoPF04dBeta = cms.vdouble(0, 0.15)
                elif D == "dBeta_025": DEN.combRelIsoPF04dBeta = cms.vdouble(0, 0.25)
            	elif D == "TrkIso_010": DEN.relTkIso = cms.vdouble(0, 0.10) ###
                elif D == "L1SingleMu16": 
                    DEN.l1pt = cms.vdouble(16, 9999)
                    DEN.l1q = cms.vdouble(5, 999)
                # Set D as the variable of DEN ... DEN.D = cms.vstring("pass")
                else: setattr(DEN, D, cms.vstring("pass"))

        # numString: EfficiencyCategoryState variable. 
        # ex> cmsRun TriggerEff.py mc IsoMu20_and_Tight2012_from_SIP4_and_PFIso25 => numString = cms.vstring("IsoMu20", "pass", "Tight2012", "pass")
        numString = cms.vstring()
        for N in num.split("_and_"):
            if N == "IsoMu20_OR_IsoTkMu20" or N == "L1SingleMu16" or N == "IsoMu24_OR_IsoTkMu24": ###
                numString += [N, "above"]
            else:
                numString += [N, "pass"]
        
        #Set Efficiency
        setattr(module.Efficiencies, ID+"_"+X, cms.PSet(
            EfficiencyCategoryAndState = numString,
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = DEN,
            BinToPDFmap = cms.vstring(shape) # ex> BinToPDFmap = cms.vstring(shape, "*pt_bin0", "voigtPlusExpo")
        ))

        #Add mcTure Efficinecy when mc fitting
        if scenario.find("mc") != -1:
            setattr(module.Efficiencies, ID+"_"+X+"_mcTrue", cms.PSet(
                EfficiencyCategoryAndState = numString,
                UnbinnedVariables = cms.vstring("mass"),
                BinnedVariables = DEN.clone(mcTrue = cms.vstring("true"))
            ))
            #When mc is PU-weighted, "weight" variable is added to UnbinnedVariables
            if "_weight" in scenario:
                getattr(module.Efficiencies, ID+"_"+X          ).UnbinnedVariables.append("weight")
                getattr(module.Efficiencies, ID+"_"+X+"_mcTrue").UnbinnedVariables.append("weight")

        #Add module to the process
        setattr(process, "TnP_MuonID_"+ID+"_"+X, module)        
        #Add a path of module to the process
        setattr(process, "run_"+ID+"_"+X, cms.Path(module))
