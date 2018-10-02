/*
 * Copyright (C) 2017 Damir Porobic <https://github.com/damirporobic>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "ScaleDialog.h"

namespace kImageAnnotator {

ScaleDialog::ScaleDialog(int width, int height, QWidget *parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
	setWindowTitle(tr("Scale Image"));

	mWidth = width;
	mHeight = height;

	initGui();
}

ScaleDialog::~ScaleDialog()
{
	delete mKeepAspectRatioCheckBox;
	delete mWidthPixelLabel;
	delete mHeightPixelLabel;
	delete mWidthPercentLabel;
	delete mHeightPercentLabel;
	delete mWidthPixelSpinBox;
	delete mHeightPixelSpinBox;
	delete mWidthPercentSpinBox;
	delete mHeightPercentSpinBox;
	delete mOkButton;
	delete mCancelButton;
	delete mPixelGridLayout;
	delete mPercentGridLayout;
	delete mPixelGroupBox;
	delete mPercentGroupBox;
	delete mButtonRowLayout;
	delete mMainLayout;
}

void ScaleDialog::initGui()
{
	mKeepAspectRatioCheckBox = new QCheckBox;
	mKeepAspectRatioCheckBox->setText(tr("Keep Aspect Ratio"));
	mKeepAspectRatioCheckBox->setChecked(true);

	mWidthPixelLabel = new QLabel;
	mWidthPixelLabel->setText(tr("Width:"));

	mHeightPixelLabel = new QLabel;
	mHeightPixelLabel->setText(tr("Height:"));

	mWidthPercentLabel = new QLabel;
	mWidthPercentLabel->setText(tr("Width:"));

	mHeightPercentLabel = new QLabel;
	mHeightPercentLabel->setText(tr("Height:"));

	mWidthPixelSpinBox = new CustomSpinBox(this);
	mWidthPixelSpinBox->setSuffix(QStringLiteral("px"));
	mWidthPixelSpinBox->setMinimum(1);
	mWidthPixelSpinBox->setMaximum(4000);
	mWidthPixelSpinBox->setValueSilent(mWidth);
	mWidthPixelSpinBox->setWrapping(false);
	connect(mWidthPixelSpinBox, &CustomSpinBox::valueChanged, [this](int newValue)
	{
		auto valuePercent = calculatePercent(mWidth, newValue);
		mWidthPercentSpinBox->setValueSilent(valuePercent * 100);
		if (mKeepAspectRatioCheckBox->isChecked()) {
			setHeightInSpinBox(valuePercent);
		}
	});

	mHeightPixelSpinBox = new CustomSpinBox(this);
	mHeightPixelSpinBox->setSuffix(QStringLiteral("px"));
	mHeightPixelSpinBox->setMinimum(1);
	mHeightPixelSpinBox->setMaximum(4000);
	mHeightPixelSpinBox->setValueSilent(mHeight);
	mHeightPixelSpinBox->setWrapping(false);
	connect(mHeightPixelSpinBox, &CustomSpinBox::valueChanged, [this](int newValue)
	{
		auto valuePercent = calculatePercent(mHeight, newValue);
		mHeightPercentSpinBox->setValueSilent(valuePercent * 100);
		if (mKeepAspectRatioCheckBox->isChecked()) {
			setWidthInSpinBox(valuePercent);
		}
	});

	mWidthPercentSpinBox = new CustomSpinBox(this);
	mWidthPercentSpinBox->setSuffix(QStringLiteral("%"));
	mWidthPercentSpinBox->setMinimum(1);
	mWidthPercentSpinBox->setMaximum(400);
	mWidthPercentSpinBox->setValueSilent(100);
	mWidthPercentSpinBox->setWrapping(false);
	connect(mWidthPercentSpinBox, &CustomSpinBox::valueChanged, [this](int newValue)
	{
		auto valuePercent = calculatePercent(newValue);
		mWidthPixelSpinBox->setValueSilent(valuePercent * mWidth);
		if (mKeepAspectRatioCheckBox->isChecked()) {
			setHeightInSpinBox(valuePercent);
		}
	});

	mHeightPercentSpinBox = new CustomSpinBox(this);
	mHeightPercentSpinBox->setSuffix(QStringLiteral("%"));
	mHeightPercentSpinBox->setMinimum(1);
	mHeightPercentSpinBox->setMaximum(400);
	mHeightPercentSpinBox->setValueSilent(100);
	mHeightPercentSpinBox->setWrapping(false);
	connect(mHeightPercentSpinBox, &CustomSpinBox::valueChanged, [this](int newValue)
	{
		auto valuePercent = calculatePercent(newValue);
		mHeightPixelSpinBox->setValueSilent(valuePercent * mHeight);
		if (mKeepAspectRatioCheckBox->isChecked()) {
			setWidthInSpinBox(valuePercent);
		}
	});

	mOkButton = new QPushButton;
	mOkButton->setText(tr("OK"));
	connect(mOkButton, &QPushButton::clicked, [this]()
	{
		emit finished(mWidthPixelSpinBox->value(), mHeightPixelSpinBox->value());
		close();
	});

	mCancelButton = new QPushButton;
	mCancelButton->setText(tr("Cancel"));
	connect(mCancelButton, &QPushButton::clicked, [this]()
	{
		close();
	});

	mPixelGridLayout = new QGridLayout;
	mPixelGridLayout->addWidget(mWidthPixelLabel, 0, 0);
	mPixelGridLayout->addWidget(mWidthPixelSpinBox, 0, 1);
	mPixelGridLayout->addWidget(mHeightPixelLabel, 1, 0);
	mPixelGridLayout->addWidget(mHeightPixelSpinBox, 1, 1);

	mPercentGridLayout = new QGridLayout;
	mPercentGridLayout->addWidget(mWidthPercentLabel, 0, 0);
	mPercentGridLayout->addWidget(mWidthPercentSpinBox, 0, 1);
	mPercentGridLayout->addWidget(mHeightPercentLabel, 1, 0);
	mPercentGridLayout->addWidget(mHeightPercentSpinBox, 1, 1);

	mPixelGroupBox = new QGroupBox;
	mPixelGroupBox->setTitle(tr("Pixel"));
	mPixelGroupBox->setLayout(mPixelGridLayout);

	mPercentGroupBox = new QGroupBox;
	mPercentGroupBox->setTitle(tr("Percent"));
	mPercentGroupBox->setLayout(mPercentGridLayout);

	mButtonRowLayout = new QHBoxLayout;
	mButtonRowLayout->addWidget(mOkButton);
	mButtonRowLayout->addWidget(mCancelButton);
	mButtonRowLayout->setAlignment(Qt::AlignRight);

	mMainLayout = new QVBoxLayout;
	mMainLayout->addWidget(mKeepAspectRatioCheckBox);
	mMainLayout->addWidget(mPixelGroupBox);
	mMainLayout->addWidget(mPercentGroupBox);
	mMainLayout->addLayout(mButtonRowLayout);

	setLayout(mMainLayout);
}

float ScaleDialog::calculatePercent(int baseValue, int newValue) const
{
	return (float) newValue / (float) baseValue;
}

float ScaleDialog::calculatePercent(int newValue) const
{
	return (float) newValue / 100;
}

void ScaleDialog::setHeightInSpinBox(float percent)
{
	mHeightPixelSpinBox->setValueSilent(percent * mHeight);
	mHeightPercentSpinBox->setValueSilent(percent * 100);
}

void ScaleDialog::setWidthInSpinBox(float percent)
{
	mWidthPixelSpinBox->setValueSilent(percent * mWidth);
	mWidthPercentSpinBox->setValueSilent(percent * 100);
}

} // namespace kImageAnnotator