# Introduction #

ValidPrime is software to correct for gDNA background signal in RT-PCR. The current version has a couple of bugs in it and should be used with care. The next release will fix the StepOne parser and have improved help system.


# Details #

for now here is the help text from the program, without proper formatting:

ValidPrime Help
This text summaries briefly the principle and functioning of the ValidPrime method and the gh-validprime software. More detailed information about the method is found in the article published in Nucleic Acids Research (2012):
Henrik Laurell, Jason S. Iacovoni, Anne Abot, David Svec, Jean-José Maoret, Jean-François Arnal, and Mikael Kubista
Correction of RT-qPCR data for genomic DNA-derived signals with ValidPrime
Nucl. Acids Res. doi:10.1093/nar/gkr1259
ValidPrime has multiple objectives:
It tests the gDNA sensitivity of Gene-of-interest (GOI) qPCR assays and classify the assays into genomic DNA (gDNA)-insensitive and gDNA-sensitive GOI assays.
For the gDNA-sensitive assays, the gDNA contribution in each individual sample is calculated and the input signal (CqNA) is corrected accordingly to generate CqRNA, which corresponds to the signal derived from RNA. Up to 60% gDNA contribution is tolerated
ValidPrime replaces the traditional RT minus controls, while requiring fewer control reactions.
DNase treatment of RNA samples becomes redundant in most qPCR studies when GOI assays have been validated with ValidPrime for their gDNA sensitivity.
To run a ValidPrime experiment, 2 components are added to the traditional RT-qPCR setup:
1) A gDNA specific ValidPrime assay (VPA) targeting transcriptionally silent gDNA
2) Purified gDNA sample(s) from the same species as the RNA samples
When testing your qPCR primers for the first time with ValidPrime it is strongly advisable to include a gDNA dilution series with at least 4, preferably more samples/concentrations. This serves 2 purposes:
First, it allows to confidently identifying gDNA-insensitive assays (see ValidPrime Scoring below).
Second, it allows an estimation of the gDNA amplification efficiency of gDNA-sensitive assays which serves as base for determination whether or not signal correction can be performed with high confidence.
The accuracy of the correction is optimal with GOI assays with efficiencies identical to that of the VPA. It is possible to evaluate this parameter in the ValidPrime software (see “ValidPrime assay validation” below).
ValidPrime consists of 4 steps that must be followed in order:
The order is shown in the top toolbar, Load -> Check -> Run -> Save Files
Elements of the interface that are not a part of the current step will be inactive to aid you in following the steps. You can use the Reload button to return to the load step, and the Recheck button to return to the configuration step.
For demonstration purposes, a small example file is included in the program. Use the File Menu to Load the Demo input. Then proceed to Check and Run to see how ValidPrime works. The demo data comes from an experiment where exogenously gDNA was spiked into mouse kidney cDNA. An example file in BioMark format is available for download from the google.code site.
Loading Input
Input files must be in Stepone or BioMark/heatmap format (currently heatmap format needs to be tab-delimited text (.txt) files with periods ('.') and not commas (',') as the decimal points). Spreadsheet format (saved as .txt)., typically output from excel-type programs, is also supported. Spreadsheet files should have the Assay names as a header (1st line) and the Sample names in the first column.
The loaded file data will appear in the window on the right. The first row should show the assay names and the first column should indicate the sample names.
Be sure to check your input in the grid before proceeding, since the program may crash if you try to load a file in a format different from the format specified in the Input Format field.
Warning for StepOne input format:
Stepone format contains variation that depends upon the version of the upstream software. For now, ValidPrime may or may not work on your file. This will be fixed by the 1.0.1 release.
Checking Input/Parameters
The Validate Assays checkbox should be checked if you are testing your qPCR primers for the first time with ValidPrime. This requires a gDNA dilution series with at least 4, preferably more samples/concentrations. This serves 2 purposes :
First, it allows to confidently identifying gDNA-insensitive assays (see ValidPrime Scoring below).
Second, it allows an approximate estimation of the gDNA amplification efficiency. ValidPrime can handle replicates of dilutions series as well as replicates of one gDNA concentration. However, in the latter case, no qPCR efficiencies will be estimated since ValidPrime only calculates these based on Cq values from dilution series. The dilution series should typically span three log10, (eg. 10, 50, 250, 1000, 5000 haploid genome copy numbers). Any gDNA-sensitive assay should readily amplify 10 copies.
The sizes of most sequenced genomes (in basepairs) are available at ensemble (http://www.ensembl.org , section : Browse a genome ; Genome Statistics). A copy number calculator is available at https://www.finnzymes.fi/java_applets/copy_number_calculation.html.
Alternatively, the genome size database (http://www.genomesize.com) contains similar information for a large number of organisms, expressed as C-values (pg/haploid genome). The information in this data base is however not necessarily based on sequence data.
The Cq cutoff is used to flag input values above a threshold (flag : LOWSIGNAL). This parameter should be used with caution since the limit of detection is assay-dependent and the value will effect the number of A+ GOIs (see ValidPrime Scoring below). The default value is set to 40.
The user can manually flag cells in spreadsheets. NOAMP refers to cells with no amplification detected. EXPFAIL refers to cells in which a signal is detected but the quality of the amplification is considered inadequate. The Pass/Fail grid in the BioMark heatmap output is read and interpreted as follows:
Cq		BioMark Flag	VP Flag		Comment
GTcut		Pass		CALC		Data is used in calculations
LTcut		Fail		EXPFAIL		Data is not used
GTcut		Fail/Pass	LOWSIGNAL	Data is not used
999 (no Cq)	Fail		NOAMP		No data available
A similar Pass/Fail grid analysis for spreadsheet inputs is not implemented in the present version of ValidPrime.
In order to run ValidPrime, certain row and column labels must be selected to dictate where the gDNA Sample columns are and where the ValidPrime Assay is in the input. Input must be loaded prior to the Check step so that the row and column labels are available within the program.
If the gDNA concentrations are not the sole numeric entities with the selected row labels, uncheck the "Parse gDNA concentrations from row labels?" checkbox and enter them manually using the Check/Set [gDNA](gDNA.md) button.
The Ignore widgets allow the exclusion of specified assays (columns) and/or samples (rows) from the analysis.
ValidPrime assay validation
Based on the results from the gDNA dilution series, ValidPrime evaluates whether or not CqRNA calculation is recommended for a given assay. Minimal number of valid cells/assay in the gDNA dilution series required for High Confidence evaluation (see below).
If a GOI assay targets gDNA, qPCR against gDNA at a given concentration will generate a Cq value which is either above or below the Cq obtained with the VPA. The difference between the two Cq will generate a delta Cq. If the efficiency of the GOI-gDNA amplification is similar to the one obtained with the VPA, the delta Cq will vary little over the dilution series. Hence, the Standard Deviation (SD) of the delta Cq values will be small. ValidPrime uses the (SD) cutoff of the delta Cq values to identify high confidence assays for which a ValidPrime adjustment is performed. The default All-SD cutoff is set to 0.3 Cq and refers to if all concentrations in the standard curve are taken into account. As an option, ValidPrime can also identify aberrant data and exclude the concentration which contributes the most to the SD. This refers to the LOO (Leave-One-Out) option (not activated by default). LOO could be used if one of the gDNA concentrations appears to behave as an outlier. The minimal required number of gDNA concentrations before LOO is 5.
It is important to note that assay validation in ValidPrime does not replace the standard criteria for assay validation according to the MIQE guidelines (http://www.rdml.org/miqe.php). The purpose for assay validation in ValidPrime is merely to verify that GOI assays, gDNA-sensitive or not, behave as expected against a gDNA template and that the amplification efficiency for gDNA-sensitive assays is similar to that of the VPA.
However, even though a primer design strategy has been used that is supposed to generate gDNA-insensitive assays (such as those designed to target exons flanking a long intron or with primers that cross exon–exon junctions) the inability of a GOI assay to amplify gDNA needs to be validated experimentally
ValidPrime Scoring
ValidPrime assigns Grades based on the %DNA in each cell of the input grid. The %DNA is defined as the fraction of the total signal (expressed in %) that is derived from gDNA. The defaults are 3% or less for an A, 25% or less for a B, and 60% or less for a C. Any cell in which the %-DNA is greater than or equal to the Grade C max will be flagged as HIGHDNA and be given the grade F (Fail).
Assays that do not amplify gDNA are assigned A+/A++ or A+++, depending of the confidence level of the test. An assay which do not amplify gDNA at any concentration over an entire dilution series get a higher confidence score than an assay that is tested on only one gDNA concentration. The following weight matrix is used to define the Confidence Score.
Flag		Score
NOAMP : 	2 points
LOWSIGNAL	1 point
EXPFAIL	0 point
CALC		-1 point
The minimal score for an A+++ assay is 8 points, A++ : 5 points and A+ 2 points To minimize the risk of false positives in designation of gDNA-insensitive “A+” assays, ValidPrime gives only 1 point a to a NOAMP results in gDNA samples with less than 20 genome copies. Even though A+++ assays could be definitely considered as DNA-insensitive, we recommend a second validation (especially for A+ and A++ assays).
In the case the VPA generates a NOAMP or a LOWSIGNAL result in a sample (meaning that the level of gDNA contamination is below the level detection or the Cq value was above the cutoff), ValidPrime will add one Cq to the Cq cutoff value and use this in the calculations. These samples are labelled A