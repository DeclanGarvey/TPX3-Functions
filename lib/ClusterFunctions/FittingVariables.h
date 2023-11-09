#pragma once

namespace DetectorProperties 
{
	const double SiliconDensity=2.3333;
}

namespace SpectroscopeFitting
{
	const double ProtonMinEnergyBin = 10;
	const double ProtonMaxEnergyBin = 500;
	const int ProtonNoEnergyBins = 98;
	
	const double ElectronMinEnergyBin = 0.0;
	const double ElectronMaxEnergyBin = 25;
	const int ElectronNoEnergyBins = 5;
	const double ElectronProtonEnergyBinRatio = 2.5;
	
	const int NoStoppingPowerBins = 80;
	const double MinStoppingPowerBin = 0.0;
	const double MaxStoppingPowerBin = 40.0;
}

namespace ClassifierFitting
{
	const int NoStoppingPowerBins = 90;
	const double MinStoppingPowerBin = 0.0;
	const double MaxStoppingPowerBin = 12.0;
}

namespace OmnidirectionalFieldConstants
{
	const double AverageIncidentAngle = 46.2131;
	const double AverageEffectiveLength = 0.09325;
	const double AverageEffectiveArea = 1.3375;
}
